#define l_stick_y 7
#define l_stick_x 6

// transmission
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 9 & 10 
RF24 radio(9,10);

byte addresses[][6] = {"1Node","2Node"}; 

void setup() {
  Serial.begin(9600);
  
  // setup transmission
  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0,15);                 // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads
  radio.openWritingPipe(addresses[1]);    // Open different pipes when writing. Write on pipe 0, address 0
  radio.openReadingPipe(1,addresses[0]);  // Read on pipe 1, as address 1
  radio.startListening();                 // Start listening
  radio.powerUp();
}

void loop() {
  transmit((byte)map(analogRead(l_stick_y),0, 1024, 0, 255 ));
}

bool transmit(byte message){
  byte gotByte;                                           // Initialize a variable for the incoming response
    
  radio.stopListening();                                  // First, stop listening so we can talk.      
  
  if ( radio.write(&message,1) ){                         // Send the counter variable to the other radio 
      if(radio.available()){      
          while(radio.available() ){                      // If an ack with payload was received
              radio.read( &gotByte, 1 );                  // Read it, and display the response time
          }
      }
  }         

}
