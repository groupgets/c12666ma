 
// This code is a modified from the original sketch from Peter Jansen
// https://github.com/tricorderproject/arducordermini
// This code removes the external ADC and uses the internal ADC instead. 
// also this code just prints the output to csv output to the terminal. 

#define SPEC_GAIN        A0
//#define SPEC_EOS         NA
#define SPEC_ST          A1
#define SPEC_CLK         A2
#define SPEC_VIDEO       A3
#define WHITE_LED        A4
#define LASER_404        A5

#define SPEC_CHANNELS    256
uint16_t data[SPEC_CHANNELS];

void setup() {

  //pinMode(SPEC_EOS, INPUT);
  pinMode(SPEC_GAIN, OUTPUT);
  pinMode(SPEC_ST, OUTPUT);
  pinMode(SPEC_CLK, OUTPUT);

  pinMode(WHITE_LED, OUTPUT);
  pinMode(LASER_404, OUTPUT);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(LASER_404, LOW);
  
  
  //digitalWrite(WHITE_LED, HIGH);
  //digitalWrite(LASER_404, HIGH);

  digitalWrite(SPEC_GAIN, HIGH);
  digitalWrite(SPEC_ST, HIGH);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_GAIN, HIGH); //LOW Gain
  //digitalWrite(SPEC_GAIN, LOW); //High Gain

  //Serial.begin(9600);
  Serial.begin(115200);
}

void readSpectrometer()
{
  //int delay_time = 35;     // delay per half clock (in microseconds).  This ultimately conrols the integration time.
  int delay_time = 1;     // delay per half clock (in microseconds).  This ultimately conrols the integration time.
  int idx = 0;
  int read_time = 35;      // Amount of time that the analogRead() procedure takes (in microseconds) (different micros will have different times) 
  int intTime = 100; 
  int accumulateMode = false;
  int i;

  // Step 1: start leading clock pulses
  for (int i = 0; i < SPEC_CHANNELS; i++) {
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
  }

  // Step 2: Send start pulse to signal start of integration/light collection
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_ST, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delay_time);

  // Step 3: Integration time -- sample for a period of time determined by the intTime parameter
  int blockTime = delay_time * 8;
  long int numIntegrationBlocks = ((long)intTime * (long)1000) / (long)blockTime;
  for (int i = 0; i < numIntegrationBlocks; i++) {
    // Four clocks per pixel
    // First block of 2 clocks -- measurement
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);

    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
  }


  // Step 4: Send start pulse to signal end of integration/light collection
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_ST, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delay_time);
  digitalWrite(SPEC_CLK, HIGH);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delay_time);

  // Step 5: Read Data 2 (this is the actual read, since the spectrometer has now sampled data)
  idx = 0;
  for (int i = 0; i < SPEC_CHANNELS; i++) {
    // Four clocks per pixel
    // First block of 2 clocks -- measurement
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, LOW);

    // Analog value is valid on low transition
    if (accumulateMode == false) {
      data[idx] = analogRead(SPEC_VIDEO);
    } else {
      data[idx] += analogRead(SPEC_VIDEO);
    }
    idx += 1;
    if (delay_time > read_time) delayMicroseconds(delay_time - read_time);   // Read takes about 135uSec

    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);

    // Second block of 2 clocks -- idle
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
  }

  // Step 6: trailing clock pulses
  for (int i = 0; i < SPEC_CHANNELS; i++) {
    digitalWrite(SPEC_CLK, LOW);
    delayMicroseconds(delay_time);
    digitalWrite(SPEC_CLK, HIGH);
    delayMicroseconds(delay_time);
  }
}

void print_data()
{
  for (int i = 0; i < SPEC_CHANNELS; i++) 
  {
    Serial.print(data[i]);
    Serial.print(',');
  }
  Serial.print("\n");
}

void loop() 
{
//  digitalWrite(LASER_404, HIGH);
//  readSpectrometer();
//  digitalWrite(LASER_404, LOW);
//  print_data();
//  delay(10);
  
//  digitalWrite(WHITE_LED, HIGH);
//  readSpectrometer();
//  digitalWrite(WHITE_LED, LOW);
//  print_data();
//  delay(10);
  
  readSpectrometer();
  print_data();
  delay(10);   
}
