#define STEP_Pin 13
uint32_t cont = 0;
uint32_t microsteps_total =30;
bool MOTOR_STATE = true;

void setup()
{
  pinMode(STEP_Pin, OUTPUT);
  
  // Configuración del TIMER1
  TCCR1A = 0;                //limpia registrador A
  TCCR1B = 0;                //limpia registrador B
  TCNT1  = 0;                //Inicializa el temporizador
  OCR1A = 7811;              // carga el registrador de comparación: ( 16MHz/1024*1Hz ) -1 = 15624 = 0X3D08   --1seg
                             // carga el registrador de comparación: ( 16MHz/1024*2Hz ) -1 = 7811.5           --0.5seg
  TCCR1B |= (1 << WGM12)|(1<<CS10)|(1 << CS12);   // modo CTC, prescaler de 1024: CS12 = 1 e CS10 = 1  
  TIMSK1 |= (1 << OCIE1A);  // habilita interrupción por igualdade de comparación
}






void loop()
{
    if(cont==9)
    {
        //Se limpia los registros del timer1
        TCCR1A = 0;                //limpia registrador A
        TCCR1B = 0;                //limpia registrador B
        TCNT1  = 0;                //Inicializa el temporizador

        // ejecuta otra acción
        digitalWrite(STEP_Pin, digitalRead(STEP_Pin) ^ 1);
        delay(5000);
        digitalWrite(STEP_Pin, digitalRead(STEP_Pin) ^ 1);
        delay(5000);
        digitalWrite(STEP_Pin, digitalRead(STEP_Pin) ^ 1);
        delay(5000);

        //Se configura nuevamente los registros 
        OCR1A = 7811;              // carga el registrador de comparación: ( 16MHz/1024*1Hz ) -1 = 15624 = 0X3D08   --1seg
                                   // carga el registrador de comparación: ( 16MHz/1024*2Hz ) -1 = 7811.5           --0.5seg                          
        TCCR1B |= (1 << WGM12)|(1<<CS10)|(1 << CS12);   // modo CTC, prescaler de 1024: CS12 = 1 e CS10 = 1  
        TIMSK1 |= (1 << OCIE1A);  // habilita interrupción por igualdade de comparación
        cont++;

    }
}
ISR(TIMER1_COMPA_vect)          // interrupción por igualdade de comparación en TIMER1
{
  
    TCNT1  = 0;                       //First, set the timer back to 0 so it resets for next interrupt
    if (cont <= microsteps_total){    //10 conteos
     MOTOR_STATE = !MOTOR_STATE;      //Invert LED state
     digitalWrite(STEP_Pin,MOTOR_STATE);  //Write new state to the LED on pin D5   
    }
    else{
     digitalWrite(STEP_Pin,false); 
    }
    cont++;


}
