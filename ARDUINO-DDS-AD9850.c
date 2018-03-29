#define AD9850_CLOCK 125000000

#define RESET_A 5
#define DATA_A  6
#define FQ_UD_A 7
#define W_CLK_A 8

#define RESET_B 9
#define DATA_B  10
#define FQ_UD_B 11
#define W_CLK_B 12

#define pulseHigh(pin) { digitalWrite(pin, HIGH); digitalWrite(pin, LOW); }

void tfr_byte_A(byte data) {
  for (int i = 0; i < 8; i++, data >>= 1) {
    digitalWrite(DATA_A, data & 0x01);
    pulseHigh(W_CLK_A);
  }
}

void tfr_byte_B(byte data) {
  for (int i = 0; i < 8; i++, data >>= 1) {
    digitalWrite(DATA_B, data & 0x01);
    pulseHigh(W_CLK_B);
  }
}

void sendFrequency_A(double frequency) {
  int32_t freq = frequency * 4294967295/AD9850_CLOCK;
  for (int b = 0; b < 4; b++, freq >>= 8) {
    tfr_byte_A(freq & 0xFF);
  }
  tfr_byte_A(0x00);
  pulseHigh(FQ_UD_A);
}

void sendFrequency_B(double frequency, int phase) {
  int32_t freq = frequency * 4294967295/AD9850_CLOCK;
  for (int b = 0; b < 4; b++, freq >>= 8) {
    tfr_byte_B(freq & 0xFF);
  }
  tfr_byte_B(phase*8);
  pulseHigh(FQ_UD_B);
}

void setup() {
  pinMode(FQ_UD_A, OUTPUT);
  pinMode(W_CLK_A, OUTPUT);
  pinMode(DATA_A, OUTPUT);
  pinMode(RESET_A, OUTPUT);

  pulseHigh(RESET_A);
  pulseHigh(W_CLK_A);
  pulseHigh(FQ_UD_A);

  pinMode(FQ_UD_B, OUTPUT);
  pinMode(W_CLK_B, OUTPUT);
  pinMode(DATA_B, OUTPUT);
  pinMode(RESET_B, OUTPUT);

  pulseHigh(RESET_B);
  pulseHigh(W_CLK_B);
  pulseHigh(FQ_UD_B);
}

void loop() {
  sendFrequency_A(1e6);

  /*
  sendFrequency_B(1e6-0.05, 0);
  for(;;);
  */

  /* This introduces delay dependent phase shifts which can be much smaller than the phase shifts introduced by the phase control word (delays have some random jitter)
  for(;;) {
    sendFrequency_B(1e6+1, 0);
    delay(1);
    sendFrequency_B(1e6, 0);
    delay(500);
  }
  */

  for(;;) {
    for(int i=0; i<32; i++) {
      sendFrequency_B(1e6, i);
      delay(100);
    }
    for(int i=0; i<32; i++) {
      sendFrequency_B(1e6, 31-i);
      delay(100);
    }
  }
}
