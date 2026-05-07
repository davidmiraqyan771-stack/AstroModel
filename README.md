# AstroModel

A real-time 2D gravitational N-body simulation written in **C**, featuring a 4th-order Runge-Kutta (RK4) numerical integrator and OpenGL rendering. The default configuration simulates the famous [three-body figure-eight orbit](https://en.wikipedia.org/wiki/Three-body_problem#Special-case_solutions) discovered by Chenciner & Montgomery.

---

## Technologies & Libraries

| Technology | Purpose |
|---|---|
| **C17** | Core programming language |
| **CMake** | Build system (≥ 3.10) |
| **OpenGL 3.3 Core** | GPU-accelerated 2D rendering |
| **GLFW 3.3** | Windowing, input, and OpenGL context management |
| **GLAD** | OpenGL function loader |
| **cglm** | Fast OpenGL-oriented matrix/vector math (view & projection matrices) |
| **POSIX Threads (pthreads)** | Multi-threaded physics ↔ rendering architecture |
| **GLSL 330** | Vertex & fragment shaders for circle rendering |

---

## Project Structure

```
AstroModel/
├── README.md
├── src/
│   ├── CMakeLists.txt              # Build configuration
│   ├── main.c                      # Entry point — window, render loop, OpenGL setup
│   ├── glad.c                      # GLAD OpenGL loader (generated)
│   │
│   ├── includes/                   # Header files
│   │   ├── buffer.h                # Triple-buffer shared state & thread API
│   │   ├── particles.h             # Particle, ParticleSystem, DerivSystem structs
│   │   ├── vector.h                # Vec2 math operations
│   │   ├── gravity.h               # Gravitational derivative computation
│   │   ├── rk4.h                   # RK4 integrator interface
│   │   ├── shader.h                # Shader program loading & uniforms
│   │   ├── dynamicMem.h            # Safe dynamic memory allocation wrappers
│   │   ├── sleep.h                 # Microsecond-precision sleep
│   │   ├── glad/                   # GLAD headers
│   │   └── KHR/                    # Khronos platform headers
│   │
│   ├── physic/                     # Physics simulation modules
│   │   ├── physicThread.c          # Physics thread — init, loop, triple-buffer writes
│   │   ├── Gravity/
│   │   │   └── gravitySolver.c     # Newtonian gravitational force computation
│   │   ├── particles/
│   │   │   └── particle.c          # Particle system lifecycle & state operations
│   │   └── rk4/
│   │       └── rungekutta4.c       # 4th-order Runge-Kutta time integrator
│   │
│   ├── render/                     # Rendering modules
│   │   └── shader/
│   │       ├── shader.c            # Shader compilation, linking & uniform setters
│   │       └── GLSLcodes/
│   │           ├── vertexShader.glsl
│   │           └── fragmentShader.glsl
│   │
│   ├── utils/                      # Utility modules
│   │   ├── dynamicMem/
│   │   │   └── dynamicMem.c        # calloc/realloc with error checking
│   │   ├── vectors/
│   │   │   └── vector.c            # Vec2 add, subtract, scale, distance²
│   │   └── sleep/
│   │       └── sleep.c             # nanosleep-based microsecond sleep
│   │
│   └── build/                      # Out-of-source build directory
```

---

## Physics & Methods

### Numerical Integration — Runge-Kutta 4 (RK4)

The simulation advances the particle system using the classical **4th-order Runge-Kutta** method. Each time step `dt`:

1. Compute derivative **k₁** at the current state
2. Compute **k₂** at state + k₁·(dt/2)
3. Compute **k₃** at state + k₂·(dt/2)
4. Compute **k₄** at state + k₃·dt
5. Combine: **Δstate = (dt/6) · (k₁ + 2·k₂ + 2·k₃ + k₄)**

This provides **O(dt⁴)** global truncation error, offering high accuracy for orbital dynamics.

### Gravitational Force

Newtonian gravity is computed pairwise between all particles:

```
F_ij = G * m_j * (r_j - r_i) / |r_j - r_i|³
```

- **G** = 39.478… (4π², normalized for AU/year/solar-mass units)
- A **softening parameter** ε is added to the distance to prevent singularities at close approach: `dist² = |Δr|² + ε²`

### Data Structures

- **`Particle`** — position (`Vec2 pos`), velocity (`Vec2 vel`), mass (`m`), radius (`r`)
- **`ParticleSystem`** — array of particles, particle count, simulation time
- **`DerivParticle`** — derivative of a particle (dpos = velocity, dvel = acceleration)
- **`DerivSystem`** — array of derivative particles for the whole system

### Default Scenario — Three-Body Figure-Eight

The simulation is initialized with the Chenciner–Montgomery figure-eight solution: three equal-mass bodies tracing a single figure-eight curve in the plane. The initial conditions use the known exact values for positions and velocities.

---

## Architecture

### Multi-Threaded Design

The application separates physics and rendering into two independent threads:

- **Main thread** — OpenGL render loop running at ~60 FPS
- **Physics thread** — runs the RK4 integrator with a very small time step (`dt = 0.00002`)

### Triple Buffering

Communication between threads uses a **lock-free triple-buffer** pattern with `atomic_int`:

- 3 buffers hold `RenderParticle` snapshots (float-precision position + radius)
- The physics thread writes to one buffer and atomically swaps it via `guardIndex`
- The render thread reads from the latest completed buffer without blocking

This eliminates mutex overhead and prevents tearing.

### Rendering Pipeline

1. Each particle is drawn as a **unit quad** (two triangles via an EBO)
2. The **vertex shader** applies per-particle model/view/projection transforms
3. The **fragment shader** discards fragments outside a radius of 0.5, rendering **circles** instead of squares
4. An orthographic projection is used, dynamically adjusted on window resize

---

## Prerequisites

Install the required development libraries (Ubuntu/Debian):

```bash
sudo apt install build-essential cmake libglfw3-dev libcglm-dev libgl-dev
```

For Fedora/RHEL:

```bash
sudo dnf install cmake glfw-devel cglm-devel mesa-libGL-devel
```

For Arch:

```bash
sudo pacman -S cmake glfw cglm mesa
```

---

## Building & Running

### 1. Create the build directory (if it doesn't exist)

```bash
cd AstroModel/src
mkdir -p build
cd build
```

### 2. Generate build files with CMake

```bash
cmake ..
```

### 3. Compile the project

```bash
make
```

### 4. Run the simulation

```bash
./astroSim
```

A window will open showing the three-body figure-eight orbit in real time. Press **Esc** to exit.

---

## Controls

| Key | Action |
|---|---|
| `Esc` | Close the simulation window |

---

## License

This project is provided as-is for educational and research purposes.