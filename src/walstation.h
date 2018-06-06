//
// Created by jamie on 6/6/18.
//

#ifndef WALSTATION_WALSTATION_H
#define WALSTATION_WALSTATION_H

#include "mbed.h"

class UsainWalstation
{
 public:
  UsainWalstation();

  void run();

 private:

  Serial pc;
  char pc_buffer[256];
  int buffer_index;
};

#endif //WALSTATION_WALSTATION_H
