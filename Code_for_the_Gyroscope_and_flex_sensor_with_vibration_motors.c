#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>
MPU6050 mpu;
Servo myservo;
int16_t ax, ay, az, gx, gy, gz;
const int FLEX_PIN = A0; // Pin connected to voltage divider output

const int FLEX_PIN1 = A1;
int motorPin = 3;
int motorPin1 = 4;
// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor
// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 37300.0; // resistance when straight
const float BEND_RESISTANCE = 90000.0; // resistance at 90 deg
void setup()
{
Serial.begin(9600);
Wire.begin();
pinMode(FLEX_PIN, INPUT);
pinMode(FLEX_PIN1, INPUT);
pinMode(motorPin, OUTPUT);
pinMode(motorPin1, OUTPUT);
mpu.initialize();
myservo.attach(9);
}
void loop()
{
// Read the ADC, and calculate voltage and resistance from it
int flexADC = analogRead(FLEX_PIN);
int flexADC1 = analogRead(FLEX_PIN1);
float flexV = flexADC * VCC / 1023.0;
float flexV1 = flexADC1 * VCC /1023.0;
float flexR = R_DIV * (VCC / flexV - 1.0);
float flexR1 = R_DIV * (VCC / flexV1 - 1.0);
Serial.println("Resistance: " + String(flexR) + " ohms");
Serial.println("Resistance1: " + String(flexR1) + " ohms");
// Use the calculated resistance to estimate the sensor's
// bend angle:
float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
0, 90.0);
float angle1 = map(flexR1, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
0, 90.0);
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
int servoPosition = map(gy, -32768, 32767, 0, 180);
myservo.write(servoPosition);
Serial.println("Bend: " + String(angle) + " degrees");
Serial.println("Bend1: " + String(angle1) + " degrees");
Serial.print("Servo Position: ");
Serial.println(servoPosition);
Serial.println();
if( flexR < 6500)
{
digitalWrite(motorPin, HIGH);
}
else
{
digitalWrite(motorPin, LOW);
}
if( flexR1 < 12000)
{
digitalWrite(motorPin1, HIGH);
}
else
{
digitalWrite(motorPin1, LOW);
}
delay(500);
}
