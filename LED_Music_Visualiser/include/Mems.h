#ifndef MEMS_H
#define MEMS_H

#include "Settings.h"
#include <stdint.h>

extern int16_t readBuffer[NB_SAMPLES];

int readDataMems(void);
void setupI2sMems(void);

#endif /*   MEMS_H   */