/* 
  This program is the interpretation routine of standard output protocol of TFmini-Plus product on Arduino.
  For details, refer to Product Specifications.
  For Arduino boards with only one serial port like UNO board, the function of software visual serial port is to be used. 
*/ 
#include <SoftwareSerial.h>  //header file of software serial port
SoftwareSerial SerialA(10,11); //define software serial port name as Serial1 and define pin2 as RX and pin3 as TX
/* For Arduinoboards with multiple serial ports like DUEboard, interpret above two pieces of code and directly use Serial1 serial port*/
int dist; //actual distance measurements of LiDAR
int strength; //signal strength of LiDAR
float temprature;
int check;  //save check value
int i;
int uart[9];  //save data measured by LiDAR
const int HEADER=0x59;  //frame header of data package
void setup() {
  Serial.begin(9600); //set bit rate of serial port connecting Arduino with computer
  SerialA.begin(115200);  //set bit rate of serial port connecting LiDAR with Arduino
}
void loop() { 
// Serial.println("Entering main loop.");
  if (SerialA.available()) {  //check if serial port has data input
    if(SerialA.read() == HEADER) {  //assess data package frame header 0x59
      uart[0]=HEADER;
      if (SerialA.read() == HEADER) { //assess data package frame header 0x59
        uart[1] = HEADER;
        for (i = 2; i < 9; i++) { //save data in array
          uart[i] = SerialA.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff)){ //verify the received data as per protocol
          dist = uart[2] + uart[3] * 256;     //calculate distance value
          strength = uart[4] + uart[5] * 256; //calculate signal strength value
          temprature = uart[6] + uart[7] *256;//calculate chip temprature
          temprature = temprature/8 - 256;
          Serial.print("dist = ");
          Serial.print(dist); //output measure distance value of LiDAR
          Serial.print('\t');
          Serial.print("strength = ");
          Serial.print(strength); //output signal strength value
          Serial.print("\t Chip Temprature = ");
          Serial.print(temprature);
          Serial.println(" celcius degree"); //output chip temperature of Lidar
        }
      }
    }
//    else Serial.println("SerialA.read() didn't match expected HEADER value.");
  }
//  else Serial.println("SerialA not available");
}
