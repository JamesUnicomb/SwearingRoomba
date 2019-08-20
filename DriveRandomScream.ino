#include <Create2.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Gaussian.h"

Create2 create(&Serial1, Create2::Baud19200);
const int BAUD_PIN = 8;
uint8_t sensors[1];
bool leftBump = false, rightBump = false, leftDrop = false, rightDrop = false;
float velocity = 0.0, omega = 0.0, dt;
float velocityLeft, velocityRight;
float sigma = 0.1, kappa = 0.05;
float wheelBase = 0.235, pi = 3.14159265;

unsigned long currentMillis;

SoftwareSerial DFSoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini DFPlayer;
Gaussian gaussian = Gaussian(0.0, 1.0);

void setup() {
  Serial.begin(9600);

  DFSoftwareSerial.begin(9600);
  DFPlayer.begin(DFSoftwareSerial);

  DFPlayer.setTimeOut(500);

  DFPlayer.volume(28);
  DFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  DFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  
  create.setBaudDefault(BAUD_PIN);

  create.start();
  create.fullMode();
}

void loop() {
  create.sensors(Create2::SensorBumpsAndWheelDrops,sensors,1);
  rightBump = (bool)bitRead(sensors[0],0);
  leftBump  = (bool)bitRead(sensors[0],1);
  rightDrop = (bool)bitRead(sensors[0],2);
  leftDrop  = (bool)bitRead(sensors[0],3);

  velocity = 0.3; omega = 0.0;
  
  setSpeed(velocity, omega);

  currentMillis = millis();
  delay(100);
  
  while (!(rightBump || leftBump)) {
    float dt = 0.001 * (millis() - currentMillis);
    currentMillis = millis();
    
    omega += 1.0 / sqrt(dt) * sigma * gaussian.random() - kappa * omega;

    setSpeed(velocity, omega);
    
    create.sensors(Create2::SensorBumpsAndWheelDrops,sensors,1);
    rightBump = bitRead(sensors[0],0);
    leftBump  = bitRead(sensors[0],1);
    rightDrop = bitRead(sensors[0],2);
    leftDrop  = bitRead(sensors[0],3);
  }

  velocity = 0.0; omega = 0.0;
  setSpeed(velocity, omega);
  
  DFPlayer.next();
  delay(2000);

  velocity = -0.2; omega = 0.0;
  setSpeed(velocity, omega);
  delay(250);
  
  velocity = 0.0; omega = 2.0;
  setSpeed(velocity, omega);
  delay(random(330, 2094));
}

void setSpeed(float velocity, float omega) {
  velocityLeft  = velocity + omega * wheelBase / 2.0;
  velocityRight = velocity - omega * wheelBase / 2.0;

  create.driveDirect((int)(1000.0 * velocityRight), (int)(1000.0 * velocityLeft));
}
