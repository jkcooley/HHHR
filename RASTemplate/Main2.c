#include "RasTemp.h"

#include <RASLib/inc/common.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/adc.h>

tBoolean led_on;
static tMotor *Motors[4];
static tBoolean initialized = false;
static tADC *adc;

void blink(void) {
    SetPin(PIN_F1, true);
    SetPin(PIN_F2, false);

    //led_on = !led_on;
}

void initMotors(void) {
    if (!initialized) {
      initialized = true;
      
      Motors[0] = InitializeServoMotor(PIN_B6, false);
      Motors[1] = InitializeServoMotor(PIN_B7, false);
      Motors[2] = InitializeServoMotor(PIN_C4, false);
      Motors[3] = InitializeServoMotor(PIN_C5, false);
   
      adc = InitializeADC(PIN_D0);
    }
}

int main(void) { 
    // float distance = 0.0f;
     blink();
     Printf("\nMotor Demo\n");
     initMotors();
     irrun();
     circle();
     runMotor();
}


void irrun(void){
//	int t=0;
        float distance = 0.0f;	
	while(1){
	Printf(
	    "IR values:  %1.3f  %1.3f  %1.3f  %1.3f\r",
            ADCRead(adc)
	);
	distance = ADCRead(adc);
	if(distance < .5f)
		blink();
	else
	{
		SetPin(PIN_F1, false);
		SetPin(PIN_F2, true);
	}
	}

// > 1 always red
// < 0 always blue
// > .99f irregular blinking
// < .99f blinking too fast to really see
// = 1 always red
//hardware issue??? 	
}



void runMotor(void){
	float left = 0, right = 0, speed = .95f, accel = .01f;
	left = speed;
	right = speed;
	SetMotor(Motors[0], left);
	SetMotor(Motors[1], left);
	SetMotor(Motors[2], right);
	SetMotor(Motors[3], right);
}


void circle(void){	
	float left = 0, right = 0, speed = .75f, accel = .01f;
	left = speed;
	right = speed;
	int t = 0;	
	SetMotor(Motors[0], left);
	SetMotor(Motors[1], left);
	SetMotor(Motors[2], right);
	SetMotor(Motors[3], right);
	while(t<1500){
		t++;
		Printf("forward");
	}
	left-=speed;
	t=0;
	setm(left,right);
	while(t<1500){
		t++;
		Printf("right");
	}
	left+=speed;
	right-=speed;
	setm(left,right);
	t=0;
	while(t<1500){
		t++;
		Printf("left");
	}
	left=-speed;
	right=-speed;
	setm(left,right);
	t=0;
	while(t<3000){
		t++;
		Printf("back");
	}
}

void go(int t){
	while(t>0){
		t--;
		Printf("moving");
	}
}

void figeight(int k){	
	float left = 0, right = 0, speed = .75f, accel = .01f;
	left=speed;
	right=speed;
	int t=0;
	setm(left,right);
	while(t<100){
		t++;
		Printf("forward");
	}
	t=0;
	right-=speed;
	setm(left,right);
	while(t<500){
		t++;
		Printf("right");
	}
	t=0;
	right+=speed;
	left-=speed;
	while(t<1000){
		t++;
		Printf("left");
	}
	t=0;
	left+=speed;
	while(t<2000){
		t++;
		Printf("diagonal");
	}
	t=0;
	right-=speed;
	setm(left,right);
	while(t<5000){
		t++;
		Printf("right");	
	}
	if(k==0){
		k++;
		figeight(k);
	}
}


void setm(float left, float right){
	
	SetMotor(Motors[0], left);
	SetMotor(Motors[1], left);
	SetMotor(Motors[2], right);
	SetMotor(Motors[3], right);
}


void stopMotors(void){
	float left = 0, right = 0;
        SetMotor(Motors[0], left);
        SetMotor(Motors[1], left);
        SetMotor(Motors[2], right);
        SetMotor(Motors[3], right);
}

//void motorDemo(void) {
//    float left = 0, right = 0, speed = 0.75f, accel = 0.01f;
//    char ch;    
//    int i;
//
//    Printf("Press:\n"
//	   "  w-forward\n"
//	   "  s-backward\n"
//	   "  a-left\n"
//	   "  d-right\n"    
//           "  i-slowly forward\n"
//	   "  k-slowly backward\n"
//	   "  j-slowly left\n"
//	   "  l-slowly right\n"    
//           "  space-stop\n"
//	   "  enter-quit\n");
//  
//    // wait for the user to enter a character
//    ch = ' ';
//    
//    while (ch != '\n') {
//        switch (ch) {
//            case 'w':
//                left = speed;
//                right = speed;
//                break;
//            case 's':
//                left = -speed;
//                right = -speed;
//                break;
//            case 'a':
//                left = -speed;
//                right = speed;
//                break;
//            case 'd':
//                left = speed;
//                right = -speed;
//                break;
//            case 'i':
//                right += accel;
//                left += accel;
//                break;
//            case 'k':
//                right -= accel;
//                left -= accel;
//                break;
//            case 'j':
//                right -= accel;
//                left += accel;
//                break;
//            case 'l':
//                right += accel;
//                left -= accel;
//                break;
//            default:
//                left = 0; 
//                right = 0;
//                break;
//        }
//
//        SetMotor(Motors[0], left);
//        SetMotor(Motors[1], left);
//        SetMotor(Motors[2], right);
//        SetMotor(Motors[3], right);
//        Printf(" set motor to %1.2f %1.2f  \r", left, right);
//        
//        ch = Getc();
//    } 
//   
//    // make sure the motors are off before exiting the demo 
//    for (i = 0; i < 4; ++i) 
//      SetMotor(Motors[i], 0);
//    Printf("\n");
//}
//
//
//void initIRSensor(void) {
//    // don't initialize this if we've already done so
//    if (initialized) {
//        return;
//    }
//    
//    initialized = true;
//
//    // initialize 4 pins to be used for ADC input
//    adc[0] = InitializeADC(PIN_D0);
//}
//


