#include <Keyboard.h>

#define BUTTONS 2
#define DEBOUNCE_DELAY 50

#define ACTIVE_CLOSED LOW
#define ACTIVE_OPENED HIGH

int button_pins[BUTTONS] = {2, 3};
int button_active_levels[BUTTONS] = {ACTIVE_CLOSED, ACTIVE_CLOSED};
int button_keycodes[BUTTONS] = {KEY_UP_ARROW, KEY_DOWN_ARROW};

void kbd_update(int ctxIndex, int newState)
{
  char keycode = button_keycodes[ctxIndex];

  if (newState == HIGH)
    Keyboard.press(keycode);
  else
    Keyboard.release(keycode);
}




int button_state[BUTTONS];
int pin_level[BUTTONS];

void button_init(int ctxIndex)
{
  pin_level[ctxIndex] = digitalRead(button_pins[ctxIndex]);
}

void button_process(int ctxIndex)
{
  int new_state = (pin_level[ctxIndex] == button_active_levels[ctxIndex])?HIGH:LOW;
  if (new_state != button_state[ctxIndex])
  {
    kbd_update(ctxIndex, new_state);
    button_state[ctxIndex] = new_state;
  }
}




int debounce_state[BUTTONS];
unsigned long debounce_time[BUTTONS];

void debounce_init(int ctxIndex)
{
  debounce_state[ctxIndex] = digitalRead(button_pins[ctxIndex]);
  debounce_time[ctxIndex] = 0;
}

void debounce_process(int ctxIndex)
{
  unsigned long tm = millis();
  int reading = digitalRead(button_pins[ctxIndex]);

  if (reading != debounce_state[ctxIndex])
  {
    debounce_time[ctxIndex] = millis();
  }

  if ((tm - debounce_time[ctxIndex]) > DEBOUNCE_DELAY)
  {
    if (reading != pin_level[ctxIndex])
    {
      pin_level[ctxIndex] = reading;
    }
  }
}




void setup()
{
  debounce_init(0);
  debounce_init(1);

  button_init(0);
  button_init(1);

  Keyboard.begin();

  pinMode(button_pins[0], INPUT);
  pinMode(button_pins[1], INPUT);
}

void loop()
{
  debounce_process(0);
  debounce_process(1);

  button_process(0);
  button_process(1);
}

