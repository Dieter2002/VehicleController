#include <stdbool.h>

#ifndef DataToMainProcessor_h
#define DataToMainProcessor_h

#pragma pack(push, 1)
struct DataToMainProcessor {
  float voltageBattery;

  int16_t disMmFront;
  int16_t disMmSideFront;
  int16_t disMmSideBack;

  int16_t potMeterValue;

  uint8_t flags; // bit 0 = state, bit 1 = isActive
} DataPacket;
#pragma pack(pop)

#endif
