#define pin_dbg1 9
#define pin_dbg2 10
#define pin_ma0 8
#define pin_ma1 6
#define pin_mb0 7
#define pin_mb1 5
#define pin_sig1 0
#define pin_sig2 1

const uint16_t pwm_threshold = 200;

volatile uint32_t pwm1_start = 0;
volatile uint32_t pwm1_value = 0;
volatile uint32_t pwm2_start = 0;
volatile uint32_t pwm2_value = 0;

// sets debug led
void set_led (uint16_t led_pin, uint8_t state) {
  digitalWrite(led_pin, state ? HIGH : LOW);
}

// drives a desired motor and toggles debug led
void drive_motor (uint16_t duty_cycle, uint16_t pin1, uint16_t pin2, uint16_t led_pin) {
  bool pin1_en = duty_cycle > 2000 - pwm_threshold && duty_cycle < 2000 + pwm_threshold;
  bool pin2_en = duty_cycle > 1000 - pwm_threshold && duty_cycle < 1000 + pwm_threshold;

  digitalWrite(pin1, pin1_en ? HIGH : LOW); 
  digitalWrite(pin2, pin2_en ? HIGH : LOW);
  set_led(led_pin, pin1_en || pin2_en);
}

// ISR callback, updates duty cycles
ISR (PCINT0_vect) {
  uint32_t current_time = micros();
  uint8_t pin_state = PINA;

  // check sig1
  if (pin_state & (1 << PINA0))
    pwm1_start = current_time;
  else
    pwm1_value = current_time - pwm1_start;

  // check sig2
  if (pin_state & (1 << PINA1))
    pwm2_start = current_time;
  else
    pwm2_value = current_time - pwm2_start;
}

// initialize pins
void setup() {
  pinMode(pin_dbg1, OUTPUT);
  pinMode(pin_dbg2, OUTPUT);
  pinMode(pin_ma0, OUTPUT);
  pinMode(pin_ma1, OUTPUT);
  pinMode(pin_mb0, OUTPUT);
  pinMode(pin_mb1, OUTPUT);
  pinMode(pin_sig1, INPUT);
  pinMode(pin_sig2, INPUT);

  noInterrupts(); // disable interrupts
  GIMSK |= (1 << PCIE0);   // enable pin change interrupts
  PCMSK0 |= (1 << PCINT0); // enable interrupt for pin_sig1
  PCMSK0 |= (1 << PCINT1); // enable interrupt for pin_sig2
  interrupts(); // enable interrupts
}

void loop() {
  noInterrupts(); // disable interrupts
  uint16_t local_pwm1 = pwm1_value;
  uint16_t local_pwm2 = pwm2_value;
  interrupts(); // enable interrupts

  // drive motors
  drive_motor(local_pwm1, pin_ma0, pin_ma1, pin_dbg1);
  drive_motor(local_pwm2, pin_mb0, pin_mb1, pin_dbg2);
}