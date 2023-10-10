#include <SPI.h>


////////////////////// PPM CONFIGURATION//////////////////////////
#define channel_number 7  //set the number of channels, in this case: 7 channels
#define sigPin 2          //set PPM signal output pin on the arduino
#define PPM_FrLen 27000   //set the PPM frame length in microseconds (1ms = 1000µs)
#define PPM_PulseLen 400  //set the pulse length
//////////////////////////////////////////////////////////////////

int ppm[channel_number];


// The sizeof this struct should not exceed 32 bytes
struct Received_data {
  byte ch1;
  byte ch2;
  byte ch3;
  byte ch4;
  byte ch5;
  byte ch6;
  byte ch7;
};

Received_data received_data;


void PPM_width_Values()
{ 
  //Here we map the received values from 1000 to 2000
  //and create the ppm signals for each channel
  ppm[0] = 1000;
  ppm[1] = 1000;
  ppm[2] = 1000;
  if(ppm[3] < 2000){
    ppm[3] = ppm[3] + 1;
  }
  ppm[4] = 1000; 
  ppm[5] = 1000;  
  ppm[6] = 1000; 
  } //MOVER ESTOS VALORES PARA VARIAR EL PPM

void reset_received_Data() 
{
  // 'safe' values to use when NO radio input is detected
  received_data.ch1 = 0;      //Throttle (channel 1) to 0 //CUIDADO CON ESTO, EL DRONE SE CAE
  received_data.ch2 = 127;
  received_data.ch3 = 127;
  received_data.ch4 = 127;
  received_data.ch5 = 0;
  received_data.ch6 = 0;
  received_data.ch7 = 0;
  
  PPM_width_Values();
}

/**************************************************/



/**************************************************/

void setup()
{
  pinMode(sigPin, OUTPUT);
  digitalWrite(sigPin, 0);  //set the PPM (D2 in this case) signal pin to the default state (off)

  //Configure the interruption registers that will create the PPM signal
  cli();
  TCCR1A = 0; // set entire TCCR1 register to 0
  TCCR1B = 0;

  OCR1A = 100;  // compare match register (not very important, sets the timeout for the first interrupt)
  TCCR1B |= (1 << WGM12);  // turn on CTC mode
  TCCR1B |= (1 << CS11);  // 8 prescaler: 0,5 microseconds at 16mhz
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  sei();

  //Call the reset data function
  reset_received_Data();

    ppm[0] = 1000;
    ppm[1] = 1000;
    ppm[2] = 1000;
    ppm[3] = 1000; 
    ppm[4] = 1000; 
    ppm[5] = 1000;  
    ppm[6] = 1000; 
}

/**************************************************/



/**************************************************/




void loop()
{
  //Create the PPM widths
  Serial.begin(250000);
  Serial.print("Throttle: "); Serial.print(received_data.ch1);  Serial.print("    ");
  Serial.print("Yaw: ");      Serial.print(received_data.ch2);       Serial.print("    ");
  Serial.print("Pitch: ");    Serial.print(received_data.ch3);     Serial.print("    ");
  Serial.print("Roll: ");     Serial.print(received_data.ch4);      Serial.print("    ");
  Serial.print("\n");
  
  PPM_width_Values();
  delay(10);

  //////////This small if will reset the data if signal is lost for 1 sec.
/////////////////////////////////////////////////////////////////////////
}//Loop end






#define clockMultiplier 2 // set this to 2 if you are using a 16MHz arduino, leave as 1 for an 8MHz arduino



//Interruption vector. here we create the PPM signal
ISR(TIMER1_COMPA_vect){
  static boolean state = true;

  TCNT1 = 0;

  if ( state ) {
    //end pulse
    PORTD = PORTD & ~B00000100; // turn pin 2 off. Could also use: digitalWrite(sigPin,0)
    OCR1A = PPM_PulseLen * clockMultiplier;
    state = false;
  }
  else {
    //start pulse
    static byte cur_chan_numb;
    static unsigned int calc_rest;

    PORTD = PORTD | B00000100; // turn pin 2 on. Could also use: digitalWrite(sigPin,1)
    state = true;

    if(cur_chan_numb >= channel_number) {
      cur_chan_numb = 0;
      calc_rest += PPM_PulseLen;
      OCR1A = (PPM_FrLen - calc_rest) * clockMultiplier;
      calc_rest = 0;
    }
    else {
      OCR1A = (ppm[cur_chan_numb] - PPM_PulseLen) * clockMultiplier;
      calc_rest += ppm[cur_chan_numb];
      cur_chan_numb++;
    }     
  }
}
