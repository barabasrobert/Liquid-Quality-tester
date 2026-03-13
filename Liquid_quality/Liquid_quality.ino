// Liquid quality meter
// Uses RGB LED and photoresistor
// Cycles through colors, reads sensor, tries to classify liquid

int pinR = A3;
int pinG = A1;
int pinB = A2;
int sensor = A0;

int counter = 0;
int sensorVal = 0;

// will store last reading for each color of illumination
int valRed = 0;
int valGreen = 0;
int valYellow = 0;
int valWhite = 0;

const int NUM_COLORS = 4; // without ledOff

// based on the liquids we expect
void (*colors[NUM_COLORS])() = {red, green, yellow, white};

void setup() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);
  Serial.println("\n=== Quality check started ===\n");
}

void loop() {
  // Turn on next color
  colors[counter]();

  delay(180); // let LED/sensor stabilize
  sensorVal = analogRead(sensor);
  delay(30);
  sensorVal = (sensorVal + analogRead(sensor)) / 2; // simple two-sample averge

  // Store value depending on active color
  switch (counter) {
  case 0:
    valRed = sensorVal;
    Serial.print("RED");
    break;
  case 1:
    valYellow = sensorVal;
    Serial.print("YELLOW");
    break;
  case 2:
    valGreen = sensorVal;
    Serial.print("GREEN");
    break;
  case 3:
    valWhite = sensorVal;
    Serial.print("WHITE");
    break;
  }
  Serial.println(sensorVal);

  ledOff();
  delay(220);

  counter++;
  if (counter >= NUM_COLORS) {
    counter = 0;

    // checking done, now classify
    classifyLiquid();
    Serial.println("------------------------");
    delay(1800);
  }
}

void classifyLiquid() {
  if (valWhite < 12 || valRed < 110) {
    Serial.println(" empty ");
    return;
  }

  // normalizations, since values are sensitive to external light/shaking
  float red_white = (float)valRed / valWhite;
  float yellow_white = (float)valYellow / valWhite;

  Serial.print(" Red-White: ");
  Serial.print(red_white, 2);
  Serial.print(" Yellow-White: ");
  Serial.println(yellow_white, 2);

  // Decision tree (tuned from pre-measured data)
  if (valRed > 480 && valYellow > 260) {
    Serial.println("DARK RED");
  } else if (valRed > 340 && red_white > 14.0) {
    Serial.println("BRIGHT RED");
  } else if (valRed > 240 && valRed < 340 && red_white > 22.0 &&
             valBlue <= 16 && valWhite <= 13) {
    Serial.println("ROSE");
  } else if (valRed > 260 && valRed < 340 && valGreen < 50 &&
             yellow_white > 1.1) {
    Serial.println("YELLOW");
  } else if (valWhite > 18 && red_white < 12.0) {
    Serial.println("COLORLESS");
  } else {
    Serial.println("UNKNOWN");
  }
}

// LED helper functions
void set_led(int r, int g, int b) {
  analogWrite(pinR, r);
  analogWrite(pinG, g);
  analogWrite(pinB, b);
}

void ledOff() { set_led(0, 0, 0); }

void red() {
  set_led(255, 0, 0);
  Serial.print("RED");
}
void green() {
  set_led(0, 255, 0);
  Serial.print("GREEN");
}
void yellow() {
  set_led(255, 255, 0);
  Serial.print("YELLOW");
}
void white() {
  set_led(255, 255, 255);
  Serial.print("WHITE");
}
