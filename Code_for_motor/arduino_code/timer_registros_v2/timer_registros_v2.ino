int STEP_Pin = 3; /* give STEP_Pin name to D3 pin */
int EN_Pin = 2;
int DIR_Pin = 7;


uint32_t cont = 0;
uint32_t cont_loop = 0;
uint32_t microsteps_total =32000;
bool DIR_STATE = true; 
bool MOTOR_STATE = true;

uint32_t aux = 0;

void setup()
{ Serial.begin(115200);
  cli(); 
  
  // Configuración del TIMER1
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  //TCNT1  = 0;                //Inicializa el temporizador
  OCR1A = 10;              // carga el registrador de comparación: ( 16MHz/1024*1Hz ) -1 = 15624 = 0X3D08   --1HZ -> 1seg
                             // carga el registrador de comparación: ( 16MHz/1024*2Hz ) -1 = 7811.5           --2HZ -> 0.5seg
  TCCR1B |= (1 << WGM12)|(1<<CS10)|(1 << CS11);   // modo CTC, prescaler de 64: CS11 = 1 e CS10 = 1  
  TIMSK1 |= (1 << OCIE1A);  // habilita interrupción por igualdade de comparación

  sei();                     //Enable back the interrupts
  
  //  
  pinMode(STEP_Pin,OUTPUT);  /*declare D3 pin as an output pin */ 
  pinMode(DIR_Pin,OUTPUT);
  pinMode(EN_Pin,OUTPUT);

  digitalWrite(EN_Pin, HIGH);
  digitalWrite(DIR_Pin, DIR_STATE); // clockwise rotation

}


void loop()
{
  if(cont_loop<3){
     //Serial.print("cont_loop: "); 
     //Serial.println(cont_loop);
   
      noInterrupts();
      Serial.println("timer_on");
     // timer_normal();
      //delay(1000);
      interrupts();
      timer_on();   
      //timer_normal(); 

      //timer_off();
      noInterrupts();
      if (cont > microsteps_total){// || ((digitalRead(Lswitch) == LOW) && (flag == 0))){
              cli();}

      Serial.println("timer_off por 5segundos"); //En 5segundos hay 10 conteos -- 5seg=10*0.5seg .......(a)
      
      cont_loop++;
      interrupts();
      timer_off();
      noInterrupts();
      delay(10000);
     } 

      if(cont_loop==3){
      Serial.println("finalización del timer");  
      while (1);      
      }
  
}

void timer_normal()
{
  //noInterrupts();
  // Configuración del TIMER1
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  TCNT1  = 0;                //Inicializa el temporizador
  OCR1A = 7811;             // carga el registrador de comparación: ( 16MHz/1024*1Hz ) -1 = 15624 = 0X3D08                           
  TCCR1B |= (1 << WGM12)|(1<<CS10)|(1 << CS12);    // modo CTC, prescaler de 1024: CS12 = 1 e CS10 = 1  
  TIMSK1 |= (1 << OCIE1A);  // habilita interrupción por igualdade de comparación       
  //cont++;
}



void timer_off()
{
  //noInterrupts();
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  TCNT1  = 0;                //Inicializa el temporizador  
}



void timer_on()
{
  sei();
  //noInterrupts();
  // Configuración del TIMER1
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  TCNT1  = 0;                //Inicializa el temporizador
  OCR1A = 10;             // carga el registrador de comparación: ( 16MHz/1024*1Hz ) -1 = 15624 = 0X3D08                           
  TCCR1B |= (1 << WGM12)|(1<<CS10)|(1 << CS11);    // modo CTC, prescaler de 1024: CS12 = 1 e CS10 = 1  
  TIMSK1 |= (1 << OCIE1A);  // habilita interrupción por igualdade de comparación 

  //cont++;
}







ISR(TIMER1_COMPA_vect)          // interrupción por igualdade de comparación en TIMER1
{
    TCNT1  = 0;                           //First, set the timer back to 0 so it resets for next interrupt
    if (cont <= microsteps_total){        // conteos
     MOTOR_STATE = !MOTOR_STATE;          //Invert LED state
     digitalWrite(STEP_Pin,MOTOR_STATE);  //Write new state to the LED on pin D5   
    }
    else{
     digitalWrite(STEP_Pin,false); 
    }

    // cada vez que se cuenta se habrá transcurrido 0.5seg .......(a)
    cont++;
     Serial.print("cont: ");
     Serial.println(cont);
}
