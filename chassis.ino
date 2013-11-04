//============================================================================
// Name        : garbagebot
// Author      : wintermute
// Version     : alpha
// Copyright is lame.
// Description : a trivial exercise in cartography/pathfinding
//============================================================================
#include <ctype.h>
#include <string.h>
#include <Wire.h>
#include <math.h>
#define I2C_TX write
#define I2C_RX read 
#define SCALE
#define HMC5883_WriteAddress 0x1E //  i.e 0x3C >> 1
#define HMC5883_ModeRegisterAddress 0x02
 
#define HMC5883_ContinuousModeCommand (uint8_t)0x00     // cast to uint8_t added to get code to compile under Arduino v1.0
 
#define HMC5883_DataOutputXMSBAddress  0x03
 
int regb=0x01;
int regbdata=0x40;
 
int outputData[6];
 
// calibrate compass
const int x_offset = 0;
const int y_offset = 0;
const int z_offset = 0;
 
#ifdef SCALE
const int x_scale = 1;
const int y_scale = 1;
const int z_scale = 1;
#endif
 



const int pingPin (6);          // ping sensor
const int laser(7);
const int aDirectional (12);        // setting up motors
const int bDirectional (13);
const int aPwm (3);                 // PWM (0-255)
const int bPwm (11);                
const int aBrake(9);                // brake on HIGH
const int bBrake(8);
const int aCurrent(A0);       // current sensing
const int bCurrent(A1);                             
int aCur;
int bCur;

                    
void setup()
{


	Serial.begin(9600); 
	// for error testing
	pinMode(aDirectional, OUTPUT);    //motor pinmodes
	pinMode(aPwm, OUTPUT);
	pinMode(bDirectional, OUTPUT);
	pinMode(bPwm, OUTPUT);
        pinMode(aBrake, OUTPUT);
        pinMode(bBrake, OUTPUT);
        pinMode(aCurrent, INPUT);
        pinMode(bCurrent, INPUT);
        pinMode(laser, OUTPUT);
        Wire.begin();


}
void loop(){
  if (Serial.available() > -1 ){
    int cmd = Serial.read();
    switch(cmd){
     case '1':
       right(255, 100);
       goto rest;
       break;
     case '2':
       reverse(255, 100);
       goto rest;
       break;
     case '3':
       forward(255, 100);
       goto rest;
       break;
     case '4':
       left(255, 100);
       goto rest;
       break;
     case '5':
       Serial.println(ping());
       break;
     case '6':
       Serial.println(read_compass());
       break;
     case '7':
       las_on();
       break;
     case '8':
       las_off();
       break;
     rest:
       brake(1);
    }
  }
}
 

int brake(int time)   // holding current
{
        digitalWrite(aBrake, HIGH);
        digitalWrite(bBrake, HIGH);
        delay(time);
        return 1;
}
int freewheel()
{
        digitalWrite(aBrake, LOW);
        digitalWrite(bBrake, LOW);
        return 1;
}


int right(int rate, int time)
{   
  // drive right
  	digitalWrite(bDirectional, HIGH);
	digitalWrite(aDirectional, LOW);
        freewheel();  //release holding current

	analogWrite(aPwm, rate);   // rate
	analogWrite(bPwm, rate);  // rate
        delay(time);//test_load();
        return 1;
}


int forward(int rate, int time)
{  // turn
	digitalWrite(bDirectional, LOW);
	digitalWrite(aDirectional, LOW);
        freewheel();   //release holding current
        analogWrite(aPwm, rate);
	analogWrite(bPwm, rate);

	delay(time);
        return 1;
}

int reverse(int rate, int time)

{ 
  // turn
  	digitalWrite(bDirectional, HIGH);
	digitalWrite(aDirectional, HIGH);
        freewheel();    //release holding current
	analogWrite(aPwm, rate);

	analogWrite(bPwm, rate);


	delay(time);
        return 1;
}

int left(int rate, int time)
{         
         // go backwards some
	digitalWrite(aDirectional, HIGH);
	digitalWrite(bDirectional, LOW);

        freewheel(); // release holding current

	analogWrite(aPwm, rate);
	analogWrite(bPwm, rate);    

	delay(time);
        return 1;
}


// sensor and methods  ... gets a little hairy!

int read_compass() {
        
  int i,x,y,z;
  double angle;
 
  Wire.beginTransmission(HMC5883_WriteAddress);
  Wire.I2C_TX(regb);
  Wire.I2C_TX(regbdata);
  Wire.endTransmission();
 
  delay(1000);
  Wire.beginTransmission(HMC5883_WriteAddress); //Initiate a transmission with HMC5883 (Write address).
  Wire.I2C_TX(HMC5883_ModeRegisterAddress);       //Place the Mode Register Address in send-buffer.
  Wire.I2C_TX(HMC5883_ContinuousModeCommand);     //Place the command for Continuous operation Mode in send-buffer.
  Wire.endTransmission();                       //Send the send-buffer to HMC5883 and end the I2C transmission.
  delay(100);
 
 
  Wire.beginTransmission(HMC5883_WriteAddress);  //Initiate a transmission with HMC5883 (Write address).
  Wire.requestFrom(HMC5883_WriteAddress,6);      //Request 6 bytes of data from the address specified.
 
  delay(500);
 
 
  //Read the value of magnetic components X,Y and Z
 
  if(6 <= Wire.available()) // If the number of bytes available for reading be <=6.
  {
    for(i=0;i<6;i++)
    {
      outputData[i]=Wire.I2C_RX();  //Store the data in outputData buffer
    }
  }
 
  x=outputData[0] << 8 | outputData[1]; //Combine MSB and LSB of X Data output register
  z=outputData[2] << 8 | outputData[3]; //Combine MSB and LSB of Z Data output register
  y=outputData[4] << 8 | outputData[5]; //Combine MSB and LSB of Y Data output register
 
#ifdef SCALE
   angle= atan2((double)y * y_scale - y_offset,(double)x * x_scale - x_offset)* (180 / 3.14159265) +180; // angle in degrees
#else
   angle= atan2((double)y - y_offset,(double)x - x_offset)* (180 / 3.14159265) +180; // angle in degrees
#endif
 
  /*
 
   Refer the following application note for heading calculation.
   http://www.ssec.honeywell.com/magnetic/datasheets/lowcost.pdf
   ----------------------------------------------------------------------------------------
   atan2(y, x) is the angle in radians between the positive x-axis of a plane and the point
   given by the coordinates (x, y) on it.
   ----------------------------------------------------------------------------------------
 
   This sketch does not utilize the magnetic component Z as tilt compensation can not be done without an Accelerometer
 
   ----------------->y
   |
   |
   |
   |
   |
   |
   \/
   x
 
 
 
         N
     NW  |  NE
   |
   W----------E
   |
     SW  |  SE
         S
 
   */
 
 
// only print the detail if NOT doing either sending either calibration data set to serial
 
  //Print the approximate direction
  Serial.print("You are heading ");
  if((angle < 22.5) || (angle > 337.5 ))
    Serial.print("South");
  if((angle > 22.5) && (angle < 67.5 ))
    Serial.print("South-West");
  if((angle > 67.5) && (angle < 112.5 ))
    Serial.print("West");
  if((angle > 112.5) && (angle < 157.5 ))
    Serial.print("North-West");
  if((angle > 157.5) && (angle < 202.5 ))
    Serial.print("North");
  if((angle > 202.5) && (angle < 247.5 ))
    Serial.print("NorthEast");
  if((angle > 247.5) && (angle < 292.5 ))
    Serial.print("East");
  if((angle > 292.5) && (angle < 337.5 ))
    Serial.print("SouthEast");
 
  Serial.print(": Angle between X-axis and the South direction ");
  if((0 < angle) && (angle < 180) )
  {
    angle=angle;
  }
  else
  {
    angle=360-angle;
  }
  Serial.print(angle);
  Serial.println(" Deg");
  Serial.print("{ ");
  Serial.print(", x = ");
  Serial.print(x);
  Serial.print(",y =  ");
  Serial.print(y);

  Serial.println(" }, ");
  delay(100);
  return(angle);
}

int ping()
{
	long duration, inches, cm;
	pinMode(pingPin, OUTPUT);

	digitalWrite(pingPin, LOW);
	delayMicroseconds(2);
	digitalWrite(pingPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(pingPin, LOW);

	pinMode(pingPin, INPUT);
	duration = pulseIn(pingPin, HIGH);

	// convert the time into a distance

	cm = microsecondsToCentimeters(duration);
	delay(100);

	return cm;
}
// rate of sound to distance conversion

long microsecondsToCentimeters(long microseconds)
{
	return microseconds / 29 / 2;
}





/*
int clearance()   // overhead clearance
{
          //turn ir emitters off
            digitalWrite(oneIRPos, HIGH);
            digitalWrite(oneIRNeg, LOW);
            digitalWrite(twoIRPos, HIGH);
            digitalWrite(twoIRNeg, LOW);
            // measure ambient infra red light
            ambientIR = analogRead(irBlister);
            delay(500);
          // pulse the IR LEDS, measure new value (badIR)
            digitalWrite(oneIRPos, LOW);
            digitalWrite(oneIRNeg, HIGH);
            digitalWrite(twoIRPos, LOW);
            digitalWrite(twoIRNeg, HIGH);  
            delay(5);
            badIR = analogRead(irBlister);
            delay(50);
          // reduce ambient infra red light 
             goodIR = badIR - ambientIR;
             Serial.print(" sensing reflected infrared at .......    ");
             Serial.println(goodIR);
             return goodIR;
}

*/

void test_load()
{
        //  a method to control motor rate/load and sense stalls
          aCur = analogRead(aCurrent);
          bCur = analogRead(bCurrent);
          Serial.print("A: ");
          Serial.println(aCur);
          Serial.print("B: ");
          Serial.println(bCur);
}

int heading()
{
  int i,avg;
       {size_t i; for(i = 0; i < 10; ++i) read_compass(); }
       avg = 0; 
       for(i = 0; i < 10; ++i){ avg = (avg * i + read_compass()) / (i + 1);}
       return avg;
}

void las_on(){digitalWrite(laser, HIGH);}
void las_off(){digitalWrite(laser, LOW);}
