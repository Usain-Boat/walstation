//
// Created by jamie on 6/6/18.
//

#include "walstation.h"

UsainWalstation::UsainWalstation() : pc(USBTX, USBRX, 9600)
{
}

void UsainWalstation::run()
{
  while (true)
  {
    if (pc.readable())
    {
      pc.gets(pc_buffer, 256);
      pc.printf("%s\n", pc_buffer);
    }
  }
}
