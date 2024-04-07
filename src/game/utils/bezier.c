#include "include/game/utils/bezier.h"

double bezier(double t, double P0, double P1, double P2, double P3) {
    return (1 - t) * (1 - t) * (1 - t) * P0 +
           3 * (1 - t) * (1 - t) * t * P1 +
           3 * (1 - t) * t * t * P2 +
           t * t * t * P3;
}