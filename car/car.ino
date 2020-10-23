// --- transmission ---

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>

Servo steering;

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);
                                                                           // Topology
byte addresses[][6] = {"1Node","2Node"};              // Radio pipe addresses for the 2 nodes to communicate.

void setup() {
  Serial.begin(9600);
  
  pinMode(3, OUTPUT);
  steering.attach(8);  
  steering.write(90);
  
  // Setup and configure radio

  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(2);                // Here we are sending 1-byte payloads to test the call-response speed
//  radio.openWritingPipe(addresses[0]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1,addresses[0]);      // Open a reading pipe on address 0, pipe 1
  radio.startListening();                 // Start listening
//  radio.powerUp();
}

void loop() {
  byte pipeNo;                          // Declare variables for the pipe and the byte received
  byte gotByte;
  while ( radio.available(&pipeNo)){              // Read all available payloads
    radio.read( &gotByte, sizeof(gotByte) );     
    Serial.print("Message recieved! ");
    Serial.println(gotByte);
//    Serial.print(" ");
//    Serial.println(gotByte[1]);              
                                                 // Since this is a call-response. Respond directly with an ack payload.
                                                 // Ack payloads are much more efficient than switching to transmit mode to respond to a call
    radio.writeAckPayload(pipeNo,&gotByte, sizeof(gotByte) );  // This can be commented out to send empty payloads.
//    analogWrite(3, gotByte[0]);
//    steering.write(map(gotByte[1], 0, 255, 0, 180));
  }

}
