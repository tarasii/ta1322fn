#include <Wire.h>

bool blnk = false;
unsigned long times = millis();
String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void read_dev(uint8_t addr);

void setup() {
  Wire.begin();
 
  Serial.begin(115200);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C TA1322"); 
  pinMode(LED_BUILTIN, OUTPUT);

  inputString.reserve(10);
  read_dev(0x61);
  SetFrequency(0x60, 1200);
  read_dev(0x61);
  //digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if(millis() - times  >= 200){
    if (blnk)
      digitalWrite(LED_BUILTIN, HIGH);
    else
      digitalWrite(LED_BUILTIN, LOW);

    times = millis();
    blnk = !blnk;
  }
  if (stringComplete) {
    Serial.println("Set freq:");
    Serial.println(inputString);
    SetFrequency(0x60, inputString.toInt());
    delay(10);
    read_dev(0x61);
    //Serial.println(inputString.toInt());
    // clear the string:
    inputString = "";
    stringComplete = false;
  }  

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    int tmp = int(inChar);
    if ((tmp>=48)&&(tmp<=57)) 
      inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

bool SetFrequency(uint8_t addr, uint16_t f){
  unsigned int _Delitel;
  byte _DelitelH;
  byte _DelitelL;  
  _Delitel=f*8+3836; //считаем делитель
  _DelitelH=_Delitel>>8;
  _DelitelL=_Delitel&0XFF;
  Wire.beginTransmission(addr);   // стучимся к синтезатору и передаем байты параметров
  Wire.write(_DelitelH);
  Wire.write(_DelitelL);
  Wire.write(0xCE);
  Wire.write(0x00);
  bool error = Wire.endTransmission();
  if(error == 0){
    return false;
  }else{
    return true;
  }
}

void read_dev(uint8_t addr)
{
  Wire.requestFrom(addr, 1);  

  while (Wire.available()) {  
    char c = Wire.read();     
    Serial.print(c, HEX);         
  }
}
