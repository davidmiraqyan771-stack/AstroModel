#include "./particles.h"
#include "../../utils/dynamicMem/dynamicMem.h"


void creDerivSystem(DerivSystem* deriv, pCount n) {

    deriv->n = n;
    deriv->dp = creDy(n, sizeof(DerivParticle));

}

void remDerivSystem(DerivSystem* deriv) {
    free(deriv->dp);
    deriv->n = 0;
}
