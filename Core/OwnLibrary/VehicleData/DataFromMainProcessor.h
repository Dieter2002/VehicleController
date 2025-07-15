#include <stdbool.h>

#ifndef DataFromMainProcessor_h
#define DataFromMainProcessor_h

struct DataFromMainProcessor {
  bool pinChanAPin1En;
  bool pinChanAPin2En;
  bool pinChanBPin1En;
  bool pinChanBPin2En;
  uint8_t speedChanA;
  uint8_t speedChanB;
};

#endif
