#include <Wire.h>

#include <EVShield.h>
#include <EVs_EV3Touch.h>
#include <EVs_NXTTouch.h>

#include <Servo.h>


EVShield evshield(0x34, 0x36);
EVs_EV3Touch touch1;
EVs_NXTTouch touch2;

Servo pitch, yaw, roll;

int K = 1;
double p = 70, y = 90, r = 105;
long rotations = 1000;  // Define variable rotations and set


void setup() {
    Serial.begin(115200);       // start serial for output

    pitch.attach(7);
    yaw.attach(8);
    roll.attach(9);
    
    pitch.write(p);  // 20 - 160
    yaw.write(y);  // 0 - 160
    roll.write(r);

    evshield.init(SH_HardwareI2C);
    
    touch1.init(&evshield, SH_BAS1); touch2.init(&evshield, SH_BBS1);

    // reset motors
    evshield.bank_a.motorReset();
    evshield.bank_b.motorReset();

    evshield.bank_a.motorRunUnlimited(SH_Motor_1, 
                 SH_Direction_Forward,  // SH_Direction_Forward 
                 SH_Speed_Full);

    // Wait for touch
    while (true) {
        if (touch1.isPressed()) {
            evshield.bank_a.motorStop(SH_Motor_Both, 
                SH_Next_Action_BrakeHold);
            break;
        }
        delay(100);
    }
}



void loop() {
    int a = touch2.readRaw();
    bool touch_status = touch1.isPressed();
    Serial.print(touch_status);

    
    if (touch2.isPressed()) {
        evshield.bank_a.motorStop(SH_Motor_Both, 
            SH_Next_Action_BrakeHold);
        exit(0);
    }

    evshield.bank_a.motorRunDegrees(SH_Motor_Both, 
            SH_Direction_Reverse, 
            2,
            10,
            SH_Completion_Dont_Wait,
            SH_Next_Action_Brake);
    
  
//    roll.write(r);
//    yaw.write(y);
//    pitch.write(p);
//    p -= 0.3;
//    Serial.println(p);
//    y += 0.1;
        
    delay(480000 / K);
}
