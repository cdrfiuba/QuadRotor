// The sizeof this struct should not exceed 32 bytes
// This gives us up to 32 8 bits channals
struct MyData {
  int throttle;
  int yaw;
  int pitch;
  int roll;
  int AUX1;
  int AUX2;
};

MyData data;

void setup()
{
  //Start everything up
  Serial.begin(250000); 
}

// Returns a corrected value for a joystick position that takes into account
// the values of the outer extents and the middle of the joystick range.
/*int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}*/

void loop()
{
  // The calibration numbers used here should be measured 
  // for your joysticks till they send the correct values.
  data.throttle = analogRead(A2);
  data.yaw      = analogRead(A3);
  data.roll     = analogRead(A0);
  data.pitch    = analogRead(A1);

Serial.print("Throttle: "); Serial.print(data.throttle);  Serial.print("    ");
Serial.print("Yaw: ");      Serial.print(data.yaw);       Serial.print("    ");
Serial.print("Pitch: ");    Serial.print(data.pitch);     Serial.print("    ");
Serial.print("Roll: ");     Serial.print(data.roll);      Serial.print("    ");
Serial.print("Aux1: ");     Serial.print(data.AUX1);      Serial.print("    ");
Serial.print("Aux2: ");     Serial.print(data.AUX2);      Serial.print("\n");
}
