#include "include/engine/trig_lookup.h"

#include <stdio.h>

double lsin[LOOKUP_TABLE_LENGTH];
double lcos[LOOKUP_TABLE_LENGTH];

void populateLookupTables()
{
    for (int i = 0; i < LOOKUP_TABLE_LENGTH; i++) {
        double index = i * 2.0 * M_PI / LOOKUP_TABLE_LENGTH;
        lsin[i] = sin(index);
        lcos[i] = cos(index);
    }
}

inline double tsin(double x)
{
    double normalizedAngle = fmod(x, 2.0 * M_PI);
    if (normalizedAngle < 0) {
        normalizedAngle += 2.0 * M_PI;
    }
    int index = (int)((normalizedAngle / (2.0 * M_PI)) * LOOKUP_TABLE_LENGTH);
    return lsin[index];
}

inline double tcos(double x)
{
    double normalizedAngle = fmod(x, 2.0 * M_PI);
    if (normalizedAngle < 0) {
        normalizedAngle += 2.0 * M_PI;
    }
    int index = (int)((normalizedAngle / (2.0 * M_PI)) * LOOKUP_TABLE_LENGTH);
    return lcos[index];
}