#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <stdint.h>

void readDataStream(const uint8_t, uint32_t);
int readDataBluetooth(void);
void checkBTConnectionState(void);
void forceBluetoothReconnect(void);
void setupI2sBluetooth(void);

#endif /*   BLUETOOTH_H   */