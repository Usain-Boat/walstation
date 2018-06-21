#include "mbed.h"
#include "usain_network.h"

//DigitalOut led1(LED1);
//DigitalOut led(LED3);
RawSerial pc(USBTX, USBRX); // tx, rx

void tx_handler()
{
//  led = !led;
}

void rx_handler(const UsainNetworkMessage &message, UsainNetwork *network)
{
//  led1 = !led1;
  char send_buffer[255] = "";
  char number[6] = "";

  switch (message.get_type())
  {
    case UsainNetworkMessage::GET:
      strcat(send_buffer, "GET ");
      break;

    case UsainNetworkMessage::POST:
      strcat(send_buffer, "POST ");
      break;

    case UsainNetworkMessage::RESP:
      strcat(send_buffer, "RESP ");
      break;

    case UsainNetworkMessage::BCST:
      strcat(send_buffer, "BCST ");
      break;

    default:
      break;
  }

  // seq
  strcat(send_buffer, "0 ");

//  // source
  itoa(message.get_source(), number, 10);
  strcat(send_buffer, number);
  strcat(send_buffer, " ");

//  // dest
  itoa(message.get_destination(), number, 10);
  strcat(send_buffer, number);
  strcat(send_buffer, " ");

//  // size
  itoa(message.get_data_size(), number, 10);
  strcat(send_buffer, number);
  strcat(send_buffer, " ");

  // data
  strcat(send_buffer, reinterpret_cast<const char *>(message.get_data()));

  printf("%s\r\n", send_buffer);
}

void main_thread()
{
  printf("start\n");

  UsainNetwork network;
  char buffer[255];
  int index = 0;

  if (!network.init())
  {
    error("error: no radio connected\n");
  }

  network.register_message_received(callback(rx_handler));
  network.register_message_send(callback(tx_handler));

  while (true)
  {
    if (pc.readable())
    {
      buffer[index] = static_cast<char>(pc.getc());

      if (buffer[index++] == '\r')
      {
        buffer[index - 1] = 0;

        // parse string
        UsainNetworkMessage to_send;
        char *pch;
        int message_token = 0;
        char tmp[246];

        strcpy(tmp, buffer);

        pch = strtok(tmp, " ");

        if (strcmp(pch, "GET") == 0)
        {
          to_send.set_type(UsainNetworkMessage::GET);
        } else if (strcmp(pch, "POST") == 0)
        {
          to_send.set_type(UsainNetworkMessage::POST);
        } else if (strcmp(pch, "RESP") == 0)
        {
          to_send.set_type(UsainNetworkMessage::RESP);
        }

        message_token++;

        while (pch != NULL)
        {
          pch = strtok(NULL, " ");

          if(strlen(pch) == 0) break;

          switch (message_token++)
          {
            case 1:
              // sequence
              break;

            case 2:
              to_send.set_source(static_cast<uint8_t>(atoi(pch)));
              break;

            case 3:
              to_send.set_destination(static_cast<uint8_t>(atoi(pch)));
              break;

            case 4:
              // size
              break;

            default:
              // data
              to_send.add_parameter(pch);
              break;
          }
        }

        network.send(to_send);

        // reset string
        memset(buffer, 0, 255);
        index = 0;
      }
    }
  }
}

int main()
{
  Thread t(osPriorityAboveNormal, 10000);

  t.start(main_thread);

  t.join();
}
