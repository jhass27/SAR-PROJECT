int STEP_Pin = 3; /* give STEP_Pin name to D3 pin */
int EN_Pin = 2;
int DIR_Pin = 7;
int Lswitch = 6; 
int flag = 0; 
int led = 8; 

// the riels have 2.10m=2100mm 
uint32_t milimeter = 70 ;
uint32_t microstep = 387.44*milimeter + 706.3; //12800; //8533; //25600;
uint8_t n_turns = 1;
volatile uint32_t cont = 0;
volatile uint32_t cont_loop = 0;

bool DIR_STATE = true; // false: clockwise rotation
bool MOTOR_STATE = true;
uint32_t microsteps_total = microstep*n_turns*2; 

uint32_t aux = 0;
volatile bool timer_flag = false;

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
 
  
  //  
  pinMode(STEP_Pin,OUTPUT);  /*declare D3 pin as an output pin */ 
  pinMode(DIR_Pin,OUTPUT);
  pinMode(EN_Pin,OUTPUT);
  pinMode(Lswitch, INPUT);
  pinMode(led, OUTPUT);  

  digitalWrite(EN_Pin, HIGH);
  digitalWrite(led, HIGH);
  digitalWrite(DIR_Pin, DIR_STATE); // clockwise rotation
   sei();                     //Enable back the interrupts
}

void loop(){



     microsteps_total = microstep*n_turns*2; 
     timer_flag=false;
     
     if(!timer_flag)
        { if (cont_loop<3)
             {
                 if(cont>microsteps_total)
                  {
                    
                    
                   while(1){
                     digitalWrite(STEP_Pin,false);
                     delay(5000);
                     cont=0;
                     cont_loop++;
                     break;
                      }
                 
                   }
             }
          else
          { 
              digitalWrite(STEP_Pin,false);
              //Serial.println("end");
              DIR_STATE = false; 
              digitalWrite(DIR_Pin, DIR_STATE); 
              //Serial.println(DIR_STATE);
              timer_off();
              microsteps_total=100000;
              timer_on();}
              
              //timer_on();
              
          
           //Serial.println("finalización del timer"); 
          while(1)
                  {  

                      if (digitalRead(Lswitch) == LOW )
                        {
                          
                          //Serial.println("low");
                          digitalWrite(led, LOW);
                          DIR_STATE = true; 
                          //Serial.println(DIR_STATE);
                          digitalWrite(DIR_Pin, DIR_STATE);
                          digitalWrite(STEP_Pin,false);
                          timer_off();
                          delay(3000);
          
                          microsteps_total=7000;
                          timer_on();
                          }
                          
                       else{ 

                            if(cont==7000)
                              {cont=0;
                               cont_loop=0; }
                         
                            break;
                            }

                                
                        }}
        
                       
                
        

//  

  Serial.println(cont);
  
 // if ((cont > microsteps_total)|| (digitalRead(Lswitch) == LOW))
 // {
  //  cli();
 //   digitalWrite(led, LOW);
 //   Serial.print("Stop: ");
 // }
 // Serial.print("Cont: ");
  
}

//With the settings above, this IRS will trigger each 500ms.
ISR(TIMER1_COMPA_vect){
  timer_flag=true;
  TCNT1  = 0;         
  if (cont <= microsteps_total)
  {
   MOTOR_STATE = !MOTOR_STATE;      //Invert LED state
   digitalWrite(STEP_Pin,MOTOR_STATE);  //Write new state to the LED on pin D5 
   cont++;  
   
  }


  
  //else{
   

   //digitalWrite(STEP_Pin,false); 
 
  //}
 
}



void timer_on()
{
  noInterrupts();
  // Configuración del TIMER1
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  //TCNT1  = 0;                //Inicializa el temporizador
  OCR1A = 10;             // carga el registrador de comparación: ( 16MHz/1024*1Hz ) -1 = 15624 = 0X3D08                           
  TCCR1B |= B00000011;//TCCR1B |= (1 << WGM12)|(1<<CS11)|(1 << CS10);   // modo CTC, prescaler de 1024: CS12 = 1 e CS10 = 1  
  TIMSK1 |= B00000010;//TIMSK1 |= (1 << OCIE1A);  // habilita interrupción por igualdade de comparación       
  interrupts();
  //cont++;
}


void timer_off()
{
  noInterrupts();
  TCNT1  = 0;
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  interrupts();
  //                //Inicializa el temporizador  

}
