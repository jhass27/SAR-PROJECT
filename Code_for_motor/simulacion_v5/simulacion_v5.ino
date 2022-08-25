
int STEP_Pin = 3; /* give STEP_Pin name to D3 pin */
int EN_Pin = 2;
int DIR_Pin = 7;
int Lswitch = 6; 
int flag = 0; 
int led = 8; 

uint32_t cont = 0;
uint32_t cont_loop = 0;
uint8_t n_turns = 1;

// the riels have 2.10m=2100mm 
uint32_t milimeter = 15 ;
uint32_t microstep = 387.44*milimeter + 706.3; //12800; //8533; //25600;
uint32_t microsteps_total = microstep*n_turns*2; 

bool DIR_STATE = false; // false: clockwise rotation
bool MOTOR_STATE = true;
uint32_t aux = 0;


void setup()
{ Serial.begin(115200);

  cli();     
  // Configuración del TIMER1
  //TCCR1A = 0;                //limpia registrador A
  //TCCR1B = 0;                //limpia registrador B
  //TCNT1  = 0;                //Inicializa el temporizador
                             // carga el registrador de comparación: ( 16MHz/1024*2Hz ) -1 = 7811.5           --2HZ -> 0.5seg
  //TCCR1B |= B00000011;//TCCR1B |= (1 << WGM12)|(1<<CS11)|(1 << CS10);   // modo CTC, prescaler de 1024: CS12 = 1 e CS10 = 1  
  //TIMSK1 |= B00000010; //TIMSK1 |= (1 << OCIE1A);  // habilita interrupción por igualdade de comparación
  //OCR1A = 10;//7811;              // carga el registrador de comparación: ( 16MHz/1024*1Hz ) -1 = 15624 = 0X3D08   --1HZ -> 1seg
  

  pinMode(STEP_Pin,OUTPUT);  /*declare D3 pin as an output pin */ 
  pinMode(DIR_Pin,OUTPUT);
  pinMode(EN_Pin,OUTPUT);
  pinMode(Lswitch, INPUT);
  pinMode(led, OUTPUT);  

  digitalWrite(EN_Pin, HIGH);
  digitalWrite(led, HIGH);
  digitalWrite(DIR_Pin, DIR_STATE); // clockwise rotation
  sei();
}


  
void loop()
{ 
  if(cont_loop<5){
    
     //Serial.print("cont_loop: "); 
     //Serial.println(cont_loop);
//   Serial.println(cont);

     delay(5000);
    
//
     //Serial.println("timer_on");
     timer_on(); 
      
     delay(3000);//t
//     
//
     //Serial.println("timer_off por 5segundos"); //En 5segundos hay 10 conteos -- 5seg=10*0.5seg .......(a)
//   
     //timer_off();
// 
     //delay(5000);
      cont_loop++;
     } 

      if(cont_loop==5){
        cli();
        bool DIR_STATE = true; 
        digitalWrite(DIR_Pin, DIR_STATE); 
        microsteps_total=50000;
        sei();
        timer_on();
        delay(5000);
      //Serial.println("finalización del timer");  
        while (1);      
      }
      
  
}



ISR(TIMER1_COMPA_vect)          // interrupción por igualdade de comparación en TIMER1
{
    TCNT1  = 0;                           //First, set the timer back to 0 so it resets for next interrupt
    if (cont <= microsteps_total){        // conteos
     MOTOR_STATE = !MOTOR_STATE;          //Invert LED state
     digitalWrite(STEP_Pin,MOTOR_STATE);  //Write new state to the LED on pin D5   
//     cont++;
     //Serial.print("cont: ");
     //Serial.println(cont);
    }
    
    else{
     digitalWrite(STEP_Pin,false); 
     //digitalWrite(STEP_Pin,LOW);  //Write new state to the LED on pin D5 
     timer_off(); 
     delay(5000);
     cont = 0;
    }

    // cada vez que se cuenta se habrá transcurrido 0.5seg .......(a)
    cont++;
}




void timer_on()
{
  
  // Configuración del TIMER1
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  //TCNT1  = 0;                //Inicializa el temporizador
  OCR1A = 10;             // carga el registrador de comparación: ( 16MHz/1024*1Hz ) -1 = 15624 = 0X3D08                           
  TCCR1B |= B00000011;//TCCR1B |= (1 << WGM12)|(1<<CS11)|(1 << CS10);   // modo CTC, prescaler de 1024: CS12 = 1 e CS10 = 1  
  TIMSK1 |= B00000010;//TIMSK1 |= (1 << OCIE1A);  // habilita interrupción por igualdade de comparación       
  //cont++;
}



void timer_off()
{
  //noInterrupts();
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  //TCNT1  = 0;                //Inicializa el temporizador  

}
