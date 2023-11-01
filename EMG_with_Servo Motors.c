#include <Servo.h>
#define SAMPLE_RATE 500
#define BAUD_RATE 115200
#define INPUT_PIN A0
#define BUFFER_SIZE 128
#define SERVO_MIN 0
#define SERVO_MAX 180
#define DELAY_TIME 1000
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
int circular_buffer[BUFFER_SIZE];
int data_index, sum;
void setup() {
// Serial connection begin
Serial.begin(BAUD_RATE);
servo1.attach(2);
servo2.attach(3);
servo3.attach(4);
servo4.attach(5);
servo5.attach(6);
}
void loop() {
// Calculate elapsed time
static unsigned long past = 0;
unsigned long present = micros();
unsigned long interval = present - past;
past = present;
50
// Run timer
static long timer = 0;
timer -= interval;
// Sample and get envelop
if(timer < 0) {
timer += 1000000 / SAMPLE_RATE;
int sensor_value = analogRead(INPUT_PIN);
int signal = EMGFilter(sensor_value);
int envelop = getEnvelop(abs(signal));
Serial.print(signal);
Serial.print(",");
Serial.println(envelop);
if(envelop || signal >100){
// Close all servo motors
servo1.write(SERVO_MIN);
servo2.write(SERVO_MIN);
servo3.write(SERVO_MIN);
servo4.write(SERVO_MIN);
servo5.write(SERVO_MIN);
delay(DELAY_TIME);
}
else{
// Open all servo motors
servo1.write(SERVO_MAX);
servo2.write(SERVO_MAX);
servo3.write(SERVO_MAX);
servo4.write(SERVO_MAX);
servo5.write(SERVO_MAX);
delay(DELAY_TIME);
}
}
}
// Envelop detection algorithm
int getEnvelop(int abs_emg){
sum -= circular_buffer[data_index];
sum += abs_emg;
circular_buffer[data_index] = abs_emg;
51
data_index = (data_index + 1) % BUFFER_SIZE;
return (sum/BUFFER_SIZE) * 2;
}
// Band-Pass Butterworth IIR digital filter, generated using filter_gen.py.
// Sampling rate: 500.0 Hz, frequency: [74.5, 149.5] Hz.
// Filter is order 4, implemented as second-order sections (biquads).
float EMGFilter(float input)
{
float output = input;
{
static float z1, z2; // filter section state
float x = output - 0.05159732*z1 - 0.36347401*z2;
output = 0.01856301*x + 0.03712602*z1 + 0.01856301*z2;
z2 = z1;
z1 = x;
}
{
static float z1, z2; // filter section state
float x = output - -0.53945795*z1 - 0.39764934*z2;
output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
z2 = z1;
z1 = x;
}
{
static float z1, z2; // filter section state
float x = output - 0.47319594*z1 - 0.70744137*z2;
output = 1.00000000*x + 2.00000000*z1 + 1.00000000*z2;
z2 = z1;
z1 = x;
}
{
static float z1, z2; // filter section state
float x = output - -1.00211112*z1 - 0.74520226*z2;
output = 1.00000000*x + -2.00000000*z1 + 1.00000000*z2;
z2 = z1;
z1 = x;
}
return output;
}
