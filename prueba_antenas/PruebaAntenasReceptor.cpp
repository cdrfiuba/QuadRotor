#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
const uint64_t pipeIn = 0xE8E8F0F0E1LL; //Remember that this code is the same as in the transmitter

int num = 0;

RF24 radio(9, 10); 


void setup()
{

Serial.begin(250000); //Set the speed to 9600 bauds if you want.

radio.begin();
radio.setAutoAck(false);
radio.setDataRate(RF24_250KBPS);

radio.openReadingPipe(1,pipeIn);
//we start the radio comunication
radio.startListening();

}


unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&num,sizeof(num));
Serial.print(num);
Serial.print("\n");
lastRecvTime = millis(); //here we receive the data
num = 0;
}
}

/**************************************************/

void loop()
{
recvData();
unsigned long now = millis();
//Here we check if we've lost signal, if we did we reset the values 
if ( now - lastRecvTime > 500) {
// Signal lost?
  Serial.print("NO HAY COMUNICACION \n");
}

}

/**************************************************/
