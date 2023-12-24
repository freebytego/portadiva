#ifndef TRIG_LOOKUP_H
#define TRIG_LOOKUP_H

#include <math.h>

#define LOOKUP_TABLE_LENGTH 32768

extern double lsin[LOOKUP_TABLE_LENGTH];
extern double lcos[LOOKUP_TABLE_LENGTH];

#define m_2_pi 2 * 3.14

void populateLookupTables();
double tsin(double x);
double tcos(double x);

#endif