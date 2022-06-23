int STEP_Pin = 3; /* give STEP_Pin name to D3 pin */
int EN_Pin = 2;
int DIR_Pin = 7;
int Lswitch = 6; 
int flag = 0; 
int led = 8; 


// the riels have 2.10m=2100mm 
uint32_t Position = 5 ;//0 
uint32_t Distance = 10 ; //0 
uint32_t Product = 0 ;
uint32_t microstep = 0; //12800; //8533; //25600;
uint8_t n_turns = 1;
uint32_t cont = 0;
uint32_t treshold = 1700;

bool DIR_STATE = true; // false: clockwise rotation
bool MOTOR_STATE = true;
bool action = false;
uint32_t microsteps_total =0; 

uint32_t aux = 0;

static volatile bool timer_flag = false;
static volatile bool calibration_flag = false;


void timer_on()
{
  noInterrupts();

  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= B00000011; 
  OCR1A = 10; //(1 - DUTY_CYCLE) * steps_freq;
  TIMSK1 |= B00000010;   
  
  //ICR1 = steps_freq;
  //TCCR1B |= (1 << CS10);
  
  //if (!calibration_flag)
  //{      
  //  TIMSK1 |= (1 << OCIE1A);
  //  interrupts();
  //}  
}

void timer_off()
{
  TCCR1A = 0;
  TCCR1B = 0;
  //TIMSK1 &= (0 << OCIE1A);  
  noInterrupts();  
}


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
  
  //  //
  pinMode(STEP_Pin,OUTPUT);  /*declare D3 pin as an output pin */ 
  pinMode(DIR_Pin,OUTPUT);
  pinMode(EN_Pin,OUTPUT);
  pinMode(Lswitch, INPUT);
  pinMode(led, OUTPUT);  

  digitalWrite(EN_Pin, HIGH);
  digitalWrite(led, HIGH);
  digitalWrite(DIR_Pin, DIR_STATE); // clockwise rotation

  //Serial.println("Enter the desired position");
  //while (Serial.available() == 0){}
   //Wait for user input  } 
  //Position = Serial.parseInt();       // get the number   
  Serial.println(Position);
  //Serial.println("Enter the desired distance");
  delay(5000);
  //while (Serial.available() == 0) {}  
  //Distance = Serial.parseInt();       // get the number
  Serial.println(Distance);
  delay(5000);
  Product=Position*Distance;
  delay(5000);

   if (Product <= treshold){
  
       for ( int i=1 ; i <= Position ; i=i+1)
        { 
         timer_off(); 
         delay(5000);
         
         microstep = 387.44*Distance + 706.3;
         microsteps_total = microstep*n_turns*2; 
         
         timer_on();
         Serial.println("high") ;  
         while (1)
          {
            if (cont > microsteps_total){
              Serial.println(cont);
              break;
          }
         delay(5000);
         timer_off();  
         }
        Serial.println(i);
        } }
    else
    {
      Serial.println("Enter again the numbers: problem  p*d>=máx allowed");
    }
}
void loop(){}




//void loop()
//{
  

//}

void(* resetFunc) (void) = 0;//declare reset function at address 0
void change_direction()
    {
    digitalWrite(led, LOW);
    Serial.print("Stop: ");
    DIR_STATE = false;
    digitalWrite(EN_Pin, HIGH);
    digitalWrite(led, HIGH);
    digitalWrite(DIR_Pin, DIR_STATE); // clockwise rotation
    microsteps_total = 1628492;
    if ((cont > microsteps_total) || ((digitalRead(Lswitch) == LOW) && (flag == 0))){
       cli();
       resetFunc(); 
    }
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
