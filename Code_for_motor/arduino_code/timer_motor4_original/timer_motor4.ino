int STEP_Pin = 3; /* give STEP_Pin name to D3 pin */
int EN_Pin = 2;
int DIR_Pin = 7;
int Lswitch = 6; 
int flag = 0; 
int led = 8; 

// the riels have 2.10m=2100mm 
uint32_t milimeter = 2100 ;
uint32_t microstep = 387.44*milimeter + 706.3; //12800; //8533; //25600;
uint8_t n_turns = 1;
uint32_t cont = 0;

bool DIR_STATE = true; // false: clockwise rotation
bool MOTOR_STATE = true;
uint32_t microsteps_total = microstep*n_turns*2; 

uint32_t aux = 0;

void setup()
{
  Serial.begin(115200);
  // Pins D3 to 12.5 kHz
  cli();                      //stop interrupts for till we make the settings
  /*1. First we reset the control register to amke sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
  /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */  
  TCCR1B |= B00000011;        //Set CS12 to 1 so we get prescalar 64  
  /*3. We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
  /*4. Set the value of register A to 25*/
  OCR1A = 10;             //Finally we set compare register A to this value  
  sei();                     //Enable back the interrupts
  
  //  
  pinMode(STEP_Pin,OUTPUT);  /*declare D3 pin as an output pin */ 
  pinMode(DIR_Pin,OUTPUT);
  pinMode(EN_Pin,OUTPUT);
  pinMode(Lswitch, INPUT);
  pinMode(led, OUTPUT);  

  digitalWrite(EN_Pin, HIGH);
  digitalWrite(led, HIGH);
  digitalWrite(DIR_Pin, DIR_STATE); // clockwise rotation
}

void loop()
{
  if ((cont > microsteps_total) || ((digitalRead(Lswitch) == LOW) && (flag == 0))){
    cli();
    digitalWrite(led, LOW);
    Serial.print("Stop: ");
  }
  Serial.print("Cont: ");
  Serial.println(cont);
}

//With the settings above, this IRS will trigger each 500ms.
ISR(TIMER1_COMPA_vect){
  TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
  if (cont <= microsteps_total){
   MOTOR_STATE = !MOTOR_STATE;      //Invert LED state
   digitalWrite(STEP_Pin,MOTOR_STATE);  //Write new state to the LED on pin D5   
  }
  else{
   digitalWrite(STEP_Pin,false); 
  }
  cont++;
}
