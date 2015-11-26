//defines the pin connections
int sensePin= 5;
int LEDPin= 8;

//defines normal and threshold voltage levels
int threshold= 500;

//sets GroundPin and LEDPin as output pins, with GroundPin being set to LOW
void setup()
{
Serial.begin(9600);

pinMode(LEDPin, OUTPUT);
}

//if the reading is higher than the threshold value, then the LED is turned on
void loop()
{
int reading= analogRead(sensePin);
Serial.println(reading);
if (reading > threshold)
{
digitalWrite(LEDPin, HIGH);
delay(1000);
digitalWrite(LEDPin, LOW);
}
}
