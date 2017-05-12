#include <Wire.h>
 
// address of PCA9685 IC
#define PCF9685_ADDR 64
#define PCF9685_channels 16

// logarithmic PWM values 
uint16_t values[PCF9685_channels]={0,75,164,266,386,525,688,878,1099,1356,1657,2007,2416,2892,3448,4095};//16 Values 0-4095

//Realisation of a pseudo cyclical PWM values
uint16_t getValue(uint8_t number)
{
  while(number>=PCF9685_channels)
    number -= PCF9685_channels;

  return values[number];
}

void setup() {
  //Initialize TWI
  Wire.begin();

  //init PCF9685
  Wire.beginTransmission(PCF9685_ADDR);
  Wire.write(0x00);     //Access Registe 0
  Wire.write(0b100000); //mode1 Enable Auto Increment - Disable Sleep
  Wire.write(0b10000);  //mode2 Invert true - LED- to PWMn Pin - LED+ to 5V
  Wire.endTransmission();
}

//Write a single Channel PWM Value
void writeChannelVal(uint16_t value){
  Wire.write(0x00);       //LEDn_ON_L
  Wire.write(0x00);       //LEDn_ON_H
  Wire.write(value&0xFF); //LEDn_OFF_L
  Wire.write(value>>8);   //LEDn_OFF_H
}



void loop() {
  //After eachg iteration step the LED Position should move
  for(uint8_t iteration=0; iteration < PCF9685_channels;iteration++)
  {

    //Due to the limited Buffer of the TWI Library the Access needs to be splitted
    for(uint8_t cycle=0;cycle<4;cycle++)
    {
      Wire.beginTransmission(PCF9685_ADDR);
      //Write Register LEDn_ON_L n={0,4,8,12}
      Wire.write(0x6+16*cycle);
      for(uint16_t i=0; i< 4;i++){
        //Always write 4 LED Values
        writeChannelVal(getValue(iteration+i+4*cycle));
      }
      Wire.endTransmission();
    }

    delay(100);
  }
    
  
    
  
  
  

  
}
