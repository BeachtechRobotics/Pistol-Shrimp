/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%BEACHTECH-ROBOTICS 
%PS4 BattleBot Code for the ESP-Wroom-32 Dev broad
%Greggory Hartsfield
%July/28/2022
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
#include <PS4Controller.h>
#include <ESP32Servo.h>
#include <Math.h>

//String blueToothMAC = "f0:f5:64:3d:4f:a7";
// TeamColors Values //
int r = 255; 
int g = 0;
int b = 0;

bool safeMode = true;

Servo motor_1;
Servo motor_2;
Servo motorWeapon_1;

int motorPin_1 = 12;
int motorPin_2 = 13;
int motorWeaponPin_1 = 4;

int motorValue_1;
int motorValue_2;
int motorWeaponValue_1;

const int centerPOS = 0;                            // set to the neutral position for your motor
const int deadband = 5; 

int ADC_Max = 4096;
  
void setup() {
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  Serial.begin(115200);
  PS4.begin("f0:f5:64:3d:4f:a7");                       //This is the Unique blueTooth MAC Adress for the PS4 Controller 
  Serial.println("Ready.");
  
  motor_1.setPeriodHertz(50);
  motor_1.write(1500);                                  //Initializing ESC 1
  motor_1.attach(motorPin_1, 1000, 2000);

  motor_2.setPeriodHertz(50);
  motor_2.write(1500);                                  //Initializing ESC 2
  motor_2.attach(motorPin_2, 1000, 2000);

  motorWeapon_1.setPeriodHertz(50);
  motorWeapon_1.write(1500);                           //Initializing ESC 3
  motorWeapon_1.attach(motorWeaponPin_1, 1000, 2000);
  
  PS4.setLed(255, 255, 255);                     //Sets SafeMode Color
  PS4.sendToController();                        // Sends data set in the above
  
  Serial.print("Initializing Motors");
    delay(1000);
  Serial.print(" . ");
    delay(1000);
  Serial.print(" . ");
    delay(1000);
  Serial.print(" . ");
    delay(1000);
  Serial.println("Compleate");
}

void loop() {
  // Below has all accessible outputs from the controller
  if (PS4.isConnected()) {
    
    if (PS4.Touchpad()) {
      //Serial.println("Touch Pad Button");
      safeMode = !safeMode;                            //Inverts safe mode
      if (safeMode == true){
        PS4.setLed(255, 255, 255);                     //Sets SafeMode Color
        PS4.setRumble(0, 0); 
        PS4.sendToController();                        // Sends data set in the above
        delay(1000);                                     // Don't send data to the controller immediately, will cause buffer overflow
        PS4.sendToController();                        // Sends data set in the above
      }else {
        PS4.setLed(r, g, b);                           //Sets Team Color
        PS4.sendToController();                        // Sends data set in the above
        delay(1000);                                    // Don't send data to the controller immediately, will cause buffer overflow
      }
    }

    
    if (PS4.LStickX()||PS4.LStickY()) {

    double x = PS4.LStickX();
    double y = PS4.LStickY();

    double r = sqrt(sq(y) + sq(x));
    double t = atan(x/y);

    t = t + (0.785);                                //  0.785398 = pi/4 

    float L = r * cos(t);
    float R = r * sin(t);
    
    int i = 1;
    if (y<0) {
      i= -1;
      }else {
      i= 1;
    }

    motorValue_1 = round(L * sqrt(2) * i);
    motorValue_2 = round(R * sqrt(2) * i);
    
    
    motorValue_1 = map(motorValue_1, -256, 256, 1000, 2000);
    motor_1.write(motorValue_1);
   
    
    
    motorValue_2 = map(motorValue_2, -256, 256, 0, 180);
    motor_2.write(motorValue_2);
    
    /*
    Serial.printf("%d ,", x);
    Serial.printf("%d ,", y);
    Serial.printf("%d ,", r);
    Serial.printf("%d ,", t);
    Serial.printf("%d ,", L);
    Serial.printf("%d ,", R);
    Serial.printf("%d ,", motorValue_1);
    Serial.printf("%d\n", motorValue_2);
    */
    }


    

    if ((PS4.L2()||PS4.R2()) && (safeMode == false)) {
      
      PS4.setRumble(PS4.L2Value(), PS4.R2Value());
      PS4.sendToController();
      //Serial.printf("L2 button at %d\n", PS4.L2Value());
      //Serial.printf("R2 button at %d\n", PS4.R2Value());
      }else {
      PS4.setRumble(0, 0);
      PS4.sendToController();
    }
    
    /*
    Serial.printf("%d ,", PS4.LStickX());
    Serial.printf("%d ,", PS4.LStickY());
    Serial.printf("%d ,", PS4.RStickX());
    Serial.printf("%d ,", PS4.RStickY());
    Serial.printf("%d ,", PS4.R2Value());
    Serial.printf("%d ,", PS4.L2Value());
    Serial.printf("%d ,", motorValue_1);
    Serial.printf("%d ,", motorValue_2);
    Serial.printf("Battery Level   %d\n", PS4.Battery());
    delay(100);
    /*
    
    
    /*
    if (PS4.Right()) Serial.println("Right Button");
    if (PS4.Down()) Serial.println("Down Button");
    if (PS4.Up()) Serial.println("Up Button");
    if (PS4.Left()) Serial.println("Left Button");

    if (PS4.Square()) Serial.println("Square Button");
    if (PS4.Cross()) Serial.println("Cross Button");
    if (PS4.Circle()) Serial.println("Circle Button");
    if (PS4.Triangle()) Serial.println("Triangle Button");    
    

    if (PS4.UpRight()) Serial.println("Up Right");
    if (PS4.DownRight()) Serial.println("Down Right");
    if (PS4.UpLeft()) Serial.println("Up Left");
    if (PS4.DownLeft()) Serial.println("Down Left");

    if (PS4.L1()) Serial.println("L1 Button");
    if (PS4.R1()) Serial.println("R1 Button");

    if (PS4.Share()) Serial.println("Share Button");
    if (PS4.Options()) Serial.println("Options Button");
    if (PS4.L3()) Serial.println("L3 Button");
    if (PS4.R3()) Serial.println("R3 Button");

    if (PS4.PSButton()) Serial.println("PS Button");
    if (PS4.Touchpad()) Serial.println("Touch Pad Button");
    */
  }
}
