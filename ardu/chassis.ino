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
#define compass 0x1E



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
        Wire.beginTransmission(compass); 
        Wire.write(byte(0x02));
        Wire.write(byte(0x00));
        Wire.endTransmission();


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
       Serial.println(read_compass(), DEC);
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


// sensor methods  ... gets a little hairy!
float read_compass() {
  int x, y, z;

  // Initiate communications with compass
  Wire.beginTransmission(compass);
  Wire.write(byte(0x03));       // Send request to X MSB register
  Wire.endTransmission();

  Wire.requestFrom(compass, 6);    // Request 6 bytes; 2 bytes per axis
  if(Wire.available() <=6) {    // If 6 bytes available
    x = Wire.read() << 8 | Wire.read();
    z = Wire.read() << 8 | Wire.read();
    y = Wire.read() << 8 | Wire.read();

  }
   // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(x, y); // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
   
  // Convert radians to degrees for readability.
  double deg = heading * 180/M_PI; 

  // Output the data via the serial port.
   
  // Correct for when signs are reversed.
  /*if(heading < 0)
    heading += 2*PI;
   
  // Convert radians to degrees for readability.
  float h_deg = heading * 180/M_PI; 
  */return (heading);
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


void las_on(){digitalWrite(laser, HIGH);}
void las_off(){digitalWrite(laser, LOW);}
