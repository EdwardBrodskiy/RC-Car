#define l_stick_y 7
#define l_stick_x 6

// transmission
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

byte addresses[][6] = {"1Node","2Node"}; 

void setup() {
  Serial.begin(9600);
  
  // setup transmission
  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(2,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(2);                // Here we are sending 1-byte payloads
  radio.openWritingPipe(addresses[0]);    // Open different pipes when writing. Write on pipe 0, address 0
//  radio.openReadingPipe(1,addresses[0]);  // Read on pipe 1, as address 1
//  radio.powerUp();
}

void loop() {
  byte values = (byte)map(analogRead(l_stick_x),0, 2000, 0, 255 );//, (byte)map(analogRead(l_stick_y),0, 1024, 0, 255 )};
  transmit(values);
}

bool transmit(byte message){
  byte gotByte;                                           // Initialize a variable for the incoming response
    
  radio.stopListening();                                  // First, stop listening so we can talk.      
  
  if ( radio.write(&message,sizeof(message)) ){                         // Send the counter variable to the other radio 
//    Serial.print("Message sent! ");
//    Serial.print(message[0]);
//    Serial.print(" ");
//    Serial.println(message[1]);
      if(radio.available()){      
          while(radio.available() ){                      // If an ack with payload was received
              radio.read( &gotByte, sizeof(gotByte) );                  // Read it, and display the response time
              Serial.println(gotByte);
          }
      }
  }         

}
