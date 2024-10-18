// Include libraries
#include "RC522.h"
#include <SPI.h>
#include <Servo.h>
#include <string.h>
#include <math.h>
// Define sensor and pin constants
#define ssTrig 6
#define ssEcho 5
#define Servo1_pin 8
// Define global variables and instances
Servo servo1;
unsigned char serNum[5];
int decid = 0;
int i = 0;
int n = 0;
float sstemp;

// Setup function
void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    SPI.begin();  // Init SPI bus
    pinMode(chipSelectPin, OUTPUT);
    digitalWrite(chipSelectPin, LOW);
    pinMode(NRSTPD, OUTPUT);
    // Initialize servo
    servo1.attach(Servo1_pin);
    pinMode(Servo1_pin, OUTPUT);
    servo1.write(0);
    // Initialize sensor pins
    pinMode(ssTrig, OUTPUT);
    pinMode(ssEcho, INPUT);
    MFRC522_Init(); 
    Serial.println("(~-w-)~hi");
}
// Hex to decimal conversion function
int hex_to_dec(unsigned char* input) {
    int decid = 0;
    for (i = 0; i < 4; i++) {
        decid += input[i] * pow(16, i);
    }
    return decid;
}
// Sensor data function
float ss_sensor_data() {
    digitalWrite(ssTrig, LOW);
    delayMicroseconds(2);
    digitalWrite(ssTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(ssTrig, LOW);
    float a = float(pulseIn(ssEcho, HIGH));
    return a;
}

// Core loop function
void loop() {
    // Card status
    decid=0;
    unsigned char status;
    unsigned char str[MAX_LEN];
    //read card status/ 2 for none / 0 for yes
    status = MFRC522_Request(PICC_REQIDL, str);
    //MI_OK=0 when card is read
    if (status == MI_OK) {   
      status = MFRC522_Anticoll(str);    
      if (status == MI_OK) {
        //print id
        memcpy(serNum, str, 5);
        Serial.print("ID:");  
        ShowCardID(serNum);
        // Convert hex to decimal
        unsigned char* id = serNum;
        decid = hex_to_dec(id);
        Serial.print("DECID: ");
        Serial.println(decid);
        // Identify allowed cards
        switch (decid) {
            case -20832: // test card
            case -13725: // test pad
            case -25276: 
            case 12703:  
            case 9487:   
            case -9186:  
            case 19925: 
                Serial.println("Door Open");
                servo1.write(180); // Turning servo
                MFRC522_Halt();
                delay (800);//wait for servo
                status = MFRC522_Request(PICC_REQIDL, str);
                for (;status==MI_OK;)
                    {
                        MFRC522_Halt();
                        delay(100);
                        status = MFRC522_Request(PICC_REQIDL, str);
                    }
                for (int i = 0, b = 0;;b++) {
                    if(b >=400){
                      Serial.println("Timed out");
                      break;
                    }
                    int data = ss_sensor_data();
                    //Serial.println(data);
                    if (data > 750){
                        i++;
                        Serial.print("times reconginzed: ");
                        Serial.println(i);
                        if(i >= 3){
                          break;
                        }
                    }
                    delay(10);
                    }
                // Reset servo
                servo1.write(0);
                //RebootCountdown();
            break;
            default:
                Serial.println("Strange Card");
            break;
        }
        MFRC522_Halt();
      } 
    }
    //scan every 0.1s
    delay(100);
}
