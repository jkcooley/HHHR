#include <RASLib/inc/common.h>
#include <RASLib/inc/gpio.h>
#include <RASLib/inc/time.h>
#include <RASLib/inc/motor.h>
#include <RASLib/inc/adc.h>
//#include <RASLib/inc/pid.h>

#define Front   0
#define Left    1
#define Right   2

// Blink the LED to show we're on
static tMotor *Motors[3];
static tADC *adc[2];
tBoolean initialized = false;
tBoolean blink_on = true;

//variables for PID control
//tPID* example;  //pid module
float k_p = 4;
float k_i = 0;
float k_d = 0;

void initRobot(void) {
    if (!initialized) {
        initialized = true;

        //Initializing Motors
        Motors[Front] = InitializeServoMotor(PIN_B5, false);
        Motors[Left] = InitializeServoMotor(PIN_B0, true);
        Motors[Right] = InitializeServoMotor(PIN_B1, false);

        //Initializing Sensors
        adc[Front] = InitializeADC(PIN_D0);//ADC on front
        adc[Left] = InitializeADC(PIN_D1);//ADC on left
    }
}

void blink(void) {
    SetPin(PIN_F3, blink_on);
    blink_on = !blink_on;
}


// The 'main' function is the entry point of the program
int main(void) {
    float wallDistance = 0;
    float frontSensor = 0;
    float error = 0;
    float setpoint = 0.5f;
    float errorSum = 0;
    float errorPrev = 0;
    float errorDeriv = 0;
    initRobot();
    // Initialization code can go here
    CallEvery(blink, 0, 0.5);
    SetMotor(Motors[Left],0.0f);//slowest is 0.25
    SetMotor(Motors[Right],0.0f);//slowest is 0.25
    SetMotor(Motors[Front],0.0f);//slowest is 0.25

    //wall follow code made by Josiekate

    while (1) {
        frontSensor = ADCRead(adc[Front]);
        error = setpoint - frontSensor;
        errorSum = errorSum + error;
        errorDeriv = error - errorPrev;
        wallDistance = k_p*error + k_i*errorSum + k_d*errorDeriv;
        SetMotor(Motors[Left], 1 - wallDistance);
        SetMotor(Motors[Right], 1 + wallDistance);
//        SetMotor(Motors[Front], wallDistance);
        errorPrev = error;
    }
}
