#include "mbed.h"
#include "usain_network.h"
#include "walstation.h"

void tx_handler()
{
  printf("transmission done\n");
}

void rx_handler(const UsainNetworkMessage &message, UsainNetwork *network)
{
  printf("received type: %d\n", message.get_type());
  printf("received message: %s\n", message.get_data());

  // respond
  UsainNetworkMessage m;

  m.set_type(UsainNetworkMessage::RESP);
  m.set_data((uint8_t *) "pong", 4);

  network->send(m);
}

int main()
{
  UsainWalstation walstation;
//  UsainNetwork network;

//  if (!network.init())
//  {
//    error("error: no radio connected\n");
//  }
//
//  network.register_message_received(callback(rx_handler));
//  network.register_message_send(callback(tx_handler));

  walstation.run();
}
