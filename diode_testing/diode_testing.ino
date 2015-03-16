//Total Pixel Count
const int totPixels = 1536;
int IntArray[totPixels];
int voltPerUnit = .0049;

short si_1 = 13;
short CLKpin = 3;
short AOpin1 = A1;
//float fullVoltages[10]; //for testing purposes

//const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
//const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

//test mode es good mode
String mode = "test";

/////////////////////////
//   SETUP
/////////////////////////
void setup()
{  
  analogReadResolution(12);
  
  if(mode == "test")
  {
    pinMode(si_1, OUTPUT);
    pinMode(CLKpin, OUTPUT);
    pinMode(AOpin1, INPUT);
  
    //set all of the digital pins low
    for( int i = 0; i < 14; i++ )
    {
        digitalWrite(i, LOW);  
    }
    
    // Clock out any existing SI pulse through the ccd register:
    for(int i = 0; i< totPixels; i++)
    {
        NewClockPulse(); 
    }
  
    // Create a new SI pulse and clock out that same SI pulse through the sensor register:
    digitalWrite(si_1, HIGH);
    NewClockPulse(); 
    digitalWrite(si_1, LOW);
    
    for(int i = 0; i < totPixels; i++)
    {
        NewClockPulse(); 
    }
    
    Serial.begin(115200);
  }
}

/////////////////////////
//   LOOP
///////////////////////// 
void loop()
{
  delay(5000);
  if(mode == "test")
  {
    // Stop the ongoing integration of light quanta from each photodiode by clocking in a SI pulse 
    // into the sensors register:
    digitalWrite(si_1, HIGH);
    NewClockPulse();
    digitalWrite(si_1, LOW);

    // Next, read all 256 pixels in parallell. Store the result in the array. Each clock pulse 
    // causes a new pixel to expose its value on the two outputs:
    for(int i=0; i < totPixels; i++)
    {
        //delayMicroseconds(20);// <-- We add a delay to stabilize the AO output from the sensor
        if(i < totPixels)
        {
          IntArray[i] = analogRead(AOpin1);
        }
        
        NewClockPulse(); 
    }

    ///THIS IS NEEDED
    // Next, stop the ongoing integration of light quanta from each photodiode by clocking in a
    // SI pulse:
    digitalWrite(si_1, HIGH);
    NewClockPulse(); 
    digitalWrite(si_1, LOW);

    // Next, send the measurement stored in the array to host computer using serial (rs-232).
    // communication. This takes ~80 ms during whick time no clock pulses reaches the sensor. 
    // No integration is taking place during this time from the photodiodes as the integration 
    // begins first after the 18th clock pulse after a SI pulse is inserted:
    for(int i = 0; i < totPixels; i++)
    {
        Serial.println(IntArray[i]);
    }
    
    Serial.println("**************Done**************");

    ///THIS IS NEEDED
    // Next, a new measuring cycle is starting once 18 clock pulses have passed. At  
    // that time, the photodiodes are once again active. We clock out the SI pulse through
    // the 256 bit register in order to be ready to halt the ongoing measurement at our will
    // (by clocking in a new SI pulse):
    for(int i = 0; i < totPixels; i++)
    {
        if(i==18)
        {
            ///WHAT GOES HERE?
        }
    }    
    
    delay(1);// <-- Add 15 ms integration time
    
    //Clear the serialWindow to avoid crashing
  }
  //delay(10000);
}

/////////////////////////
//   Methods
/////////////////////////

//NewClockPulse
void NewClockPulse()
{
  delayMicroseconds(1);
  digitalWrite(CLKpin, HIGH);
  digitalWrite(CLKpin, LOW);
}

//ClockPulse
void ClockPulse()
{
  //Extend the clock pulse to allow for a longer settling time
  digitalWrite(CLKpin, HIGH);
  delayMicroseconds(80);
  digitalWrite(CLKpin, LOW);
}

/////////////////////////
//sendPulse
void sendPulse(int pin)
{
  digitalWrite(pin, HIGH);
  delayMicroseconds(12);
  digitalWrite(pin, LOW);
}

//print the data
void PrintData()
{
  for(int i = 0; i < totPixels; i++)
  {
      int test = IntArray[i];
      Serial.println(test);
  } 
}
