/*
   HomrR - ardu-photon-mb
   v1.0
   2020.11.28.

   ----------------------------

   Command format
     |6bit|| 10bit  |
     |pin || value  |
     |----||--------|
   0x0000000000000000

   ----------------------------

   Examples:
   > 12289 -> 12pin On
   > 12288 -> 12pin Off
   > 11265 -> 11pin On
   > 11264 -> 11pin OFF
*/

const String VER = "v1.0";
const unsigned int PIN_MASK = 64512;
const unsigned int VALUE_MASK = 1023;
const int MIN_PIN_NUM = 2;
const int MAX_PIN_NUM = 13;

const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

void setup() {
  Serial.begin(9600);
  Serial.println("ardu-photon-mb " + VER);

  // Set all pin as OUTPUT (except RX TX) and set them LOW
  for (int i = MIN_PIN_NUM; i <= MAX_PIN_NUM; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  Serial.println("Arduino initialized");
}

void loop() {
  handleSerialData();
  processData();
}

void handleSerialData() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void processData() {
  if (newData == true) {
    Serial.println(receivedChars);
    unsigned int command = strtoul(receivedChars, NULL, 10);
    unsigned int pin = command >> 10;
    unsigned int value = command & VALUE_MASK;
    char message[100];
    sprintf(message, "pin: %d value: %d", pin, value);
    Serial.println(message);

    if (pin >= MIN_PIN_NUM && pin <= MAX_PIN_NUM) {
      digitalWrite(pin, value);
    }

    newData = false;
  }
}
