#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Wire.h>


RF24 radio(9, 10);   // nRF24L01 (CE, CSN)
const byte address[6] = "00001"; // Address

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
};

Data_Package data; //Create a variable with the above structure

void setup() {
  Serial.begin(9600);
  // Define the radio communication
  radio.begin();
  radio.openWritingPipe(address);
  radio.setAutoAck(false);
//  radio.setDataRate(RF24_250KBPS);
//  radio.setPALevel(RF24_PA_LOW);
  
  
  
  // Set initial default values
  data.j1PotX = 127; // Values from 0 to 255. When Joystick is in resting position, the value is in the middle, or 127. We actually map the pot value from 0 to 1023 to 0 to 255 because that's one BYTE value
  data.j1PotY = 127;

}
void loop() {
  // Read all analog inputs and map them to one Byte value
  data.j1PotX = map(analogRead(A6), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.j1PotY = map(analogRead(A7), 0, 1023, 0, 255);

  Serial.print(data.j1PotX);
  Serial.print(" ");
  Serial.println(data.j1PotY);
  
  // Send the whole data from the structure to the receiver
  radio.write(&data, sizeof(Data_Package));
}
