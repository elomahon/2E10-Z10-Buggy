const unsigned int NUM_AVG = 100;
const int DIODEpin = A0;
const int LM35pin = A1;
const float V_ref = 3.2929;
const float dvdt = 0.01;// LM35 change in voltage due to temperature
void setup() {
Serial.begin(9600);
}
void loop() {
float diodeVoltage = get_reading_v(DIODEpin);
float referenceTemperature = get_reading_v(LM35pin)/dvdt;
Serial.print(diodeVoltage,6);
Serial.print(",");
Serial.println(referenceTemperature);
delay(1000);
}
float average_reading(int pin) {
unsigned long average = 0;
for (int i = 0; i < NUM_AVG; i++) {
average += analogRead(pin);
delay(1);
}
return ((float)average) / NUM_AVG;
}
float convertToVolts(float reading) {
return reading/1023.0*V_ref;
}
float get_reading_v(int pin) {
return convertToVolts(average_reading(pin));
}
