#include <SoftwareSerial.h> //generate a software channel between arduino and the bluetooth module

#include <Stepper.h>

#define PHR A5
#define pinButton 2
//define pin RX and TX da Arduino BT module 
#define BT_TX_PIN 12
#define BT_RX_PIN 13
#define MAXSTEPS 5 // max character of the string step send by the bluetooth module

const int stepsPerRevolution = 2048; // number of steps to make a complete a full rotation of the stepper motor
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);
int brightness = 0;
const double k = 5.0 / 1024;
const double luxFactor = 500000;
const double R2 = 2000;
const double B = 1.3 * pow(10.0, 7);
const double m = -1.4;
const int maxSteps = -5120; //max 2,5 rounds 
int pos = 0;
int maxLux = 80;
int n_reductive = 5;
int period = 12000;
unsigned long time_now = 0;
//BLUETOOTH 
int ind = 0;
char c;
int stepsFromB;
bool override = 0;
SoftwareSerial bt = SoftwareSerial(BT_RX_PIN, BT_TX_PIN);
char stringa[MAXSTEPS];

//**movement manager block**
int calc_steps(int lux) { // y=-0.004x+120-> formula ci dice a quanti steps dobbiamo essere 
  int steps = (int)((lux - 80) / (-0.004));
  return steps / n_reductive; //dividere per 10 se necessario per i test 
}

int move(int new_pos) {
  int how_many = 0;
  if (new_pos <= 0 && new_pos >= maxSteps) {
    myStepper.step(new_pos - pos);
    how_many = (new_pos - pos);
    pos = new_pos;
  } else if (new_pos > 0) {
    myStepper.step(-pos);
    how_many = -pos;
    pos = 0;
  } else if (new_pos < maxSteps) {
    how_many = maxSteps - pos;
    myStepper.step(how_many);
    pos = maxSteps;
  }
  return how_many;
}
//**end of the movement manager block

//interrupt function
void reset_position() {
  move(-pos);
  pos = 0;
  maxLux = analogRead(PHR);
  maxLux = light_intensity(maxLux);
}
//**light reader and translation in lumens block
int light_intensity(int RawADC0) {
  double V2 = k * RawADC0;
  double R1 = (5.0 / V2 - 1) * R2;
  double lux = B * pow(R1, m);
  return lux;

}
//**bluetooth block
void check() {
  memset(stringa, '\0', sizeof(stringa));
  while (bt.available() > 0) {

    c = (char) bt.read();
    if (c >= '0' && c <= '9' && c != 'o' && c != 's') {
      stringa[ind] = c;
      ind++;
    } else if (c == 's') { //stop 
      override = 0;
      break;
    }
    delay(20);
  }
  stepsFromB = atoi(stringa);
  ind = 0;
  if (stringa[0] != '\0') {
    Serial.println(stringa);
    Serial.print("numero=");
    Serial.println(stepsFromB);
    move(-stepsFromB);
  }
}

void setup() {
  pinMode(BT_RX_PIN, INPUT);
  pinMode(BT_TX_PIN, OUTPUT);

  pinMode(PHR, INPUT);
  pinMode(pinButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinButton), reset_position, CHANGE); // attach an interrupt to a button connected to pinBuotton , other parameter ISR=reset_position; mode: CHANGE (EVERY CHANGED TRIGGERS THE ISR) 
  maxLux = analogRead(PHR);
  maxLux = light_intensity(maxLux);
  delay(10000);
  maxLux = analogRead(PHR);
  maxLux = light_intensity(maxLux);

  myStepper.setSpeed(10);
  Serial.begin(9600); // serial comunication active 
  bt.begin(9600);
}

void loop() {
  while (bt.available() > 0) {
    c = (char) bt.read();
    if (c == 'o') {
      override = 1;
      break;
    }

  }
  delay(20);
  if (override) {
    check();
  } else {
    Serial.println(maxLux);
    brightness = analogRead(PHR);

    int lux = light_intensity(brightness);

    int new_pos = calc_steps(lux);
    time_now = millis();
    Serial.print("lux=");
    Serial.println(lux);
    Serial.print("new_pos:");
    Serial.println(new_pos);
    Serial.print("position:");
    Serial.println(pos);

    Serial.print("motion of :");
    Serial.println(move(new_pos));
    while (millis() < time_now + period) {
      if (bt.available() > 0) {
        break;
      }
    };
  }
}
