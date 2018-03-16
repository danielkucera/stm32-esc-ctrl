#include "stm32f103c8t6.h"
#include "mbed.h"
#include "USBSerial.h"
#include <mbed_events.h>

#define RST PB6
#define SWDIO PB_14
#define SWCLK PB_13
#define SWIM PB11
#define LED PA9

DigitalOut  myled(PA_9);
DigitalOut  esc_signal(SWCLK);

int speed = 0;
static EventQueue eventQueue(/* event count */ 16 * EVENTS_EVENT_SIZE);
USBSerial *serial;

void sendPulse(){
  esc_signal=1;
  wait_us(1000 + speed);
  esc_signal=0;
}

void processInput(){
  if (serial->available() == 0)
    return;

  char ch;

  myled = !myled;
  ch = serial->getc();
  switch (ch) {
    case 'a':
      speed--;
      break;
    case 'd':
      speed++;
      break;
  }
  serial->printf("speed: %d\n", speed);
}

int main(void) {
    confSysClock();

    esc_signal=1;

    serial = new USBSerial(0x1f00, 0x2012, 0x0001, true);

    eventQueue.call_every(10, processInput);

    eventQueue.call_every(20, sendPulse);

    eventQueue.dispatch_forever();

}
