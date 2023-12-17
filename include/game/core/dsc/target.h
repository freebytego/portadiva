#ifndef DSC_TARGET_H
#define DSC_TARGET_H

typedef struct dsc_target_t {
    int time;
    int type;
    int x;
    int y;
    int angle;
    int distance;
    int amplitude;
    int frequency;
} dsc_target_t;

#endif