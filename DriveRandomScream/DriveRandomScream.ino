#include <Create2.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "Gaussian.h"

// set up Roomba
Create2 create(&Serial1, Create2::Baud19200);
const int BAUD_PIN = 8;
uint8_t sensors[1];

// Variables used in the script
bool leftBump = false, rightBump = false, leftDrop = false, rightDrop = false;
float velocity = 0.0, omega = 0.0, dt;
float velocityLeft, velocityRight;

// Random walker parameters
float sigma = 0.1, kappa = 0.05;

// irobot Roomba Parameters
float wheelBase = 0.235;

unsigned long currentMillis;

SoftwareSerial DFSoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini DFPlayer;
Gaussian gaussian = Gaussian(0.0, 1.0);

void setup() {
  Serial.begin(9600);

  // set up the mini mp3 player
  DFSoftwareSerial.begin(9600);
  DFPlayer.begin(DFSoftwareSerial);

  DFPlayer.setTimeOut(500);

  DFPlayer.volume(28);
  DFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  DFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  // set up the irobot create
  create.setBaudDefault(BAUD_PIN);

  create.start();
  create.fullMode();
}

void loop() {
  // First check, make sure Roomba isnt already against a wall
  create.sensors(Create2::SensorBumpsAndWheelDrops,sensors,1);
  rightBump = (bool)bitRead(sensors[0],0);
  leftBump  = (bool)bitRead(sensors[0],1);
  rightDrop = (bool)bitRead(sensors[0],2);
  leftDrop  = (bool)bitRead(sensors[0],3);

  // set the initial velocity
  velocity = 0.3; omega = 0.0;
  setSpeed(velocity, omega);

  // create a start time
  currentMillis = millis();
  delay(100);
  
  while (!(rightBump || leftBump)) {
    float dt = 0.001 * (millis() - currentMillis);
    currentMillis = millis();

    // random gaussian noise added to angular rotation
    omega += 1.0 / sqrt(dt) * sigma * gaussian.random() - kappa * omega;

    setSpeed(velocity, omega);

    //check if the bumper has been pressed, if true then loop ends
    create.sensors(Create2::SensorBumpsAndWheelDrops,sensors,1);
    rightBump = bitRead(sensors[0],0);
    leftBump  = bitRead(sensors[0],1);
    rightDrop = bitRead(sensors[0],2);
    leftDrop  = bitRead(sensors[0],3);
  }

  // stop Roomba after bump
  velocity = 0.0; omega = 0.0;
  setSpeed(velocity, omega);

  // play swear sound
  DFPlayer.next();
  delay(2000);

  // escape manouver: drive backwards for a short distance
  velocity = -0.2; omega = 0.0;
  setSpeed(velocity, omega);
  delay(250);

  // escape manouver: turn a random angle
  velocity = 0.0; omega = 2.0;
  setSpeed(velocity, omega);
  delay(random(330, 2094));
}

void setSpeed(float velocity, float omega) {
  /* utilty function for setting left and right
     wheel velocity from the forward and angular velocities
  */
  velocityLeft  = velocity + omega * wheelBase / 2.0;
  velocityRight = velocity - omega * wheelBase / 2.0;

  create.driveDirect((int)(1000.0 * velocityRight), (int)(1000.0 * velocityLeft));
}
