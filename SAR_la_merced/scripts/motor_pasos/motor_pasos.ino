//Pines del Motor a pasos
int STEP_Pin = 3;
int EN_Pin   = 2;
int DIR_Pin  = 7;
//Pin del Limit switch
int Lswitch  = 6;
//Pines de indicadores 
int flag     = 0; 
int led      = 8; 

// the riels have 2.10m=2100mm 
// Medidas por micropaso 
uint32_t milimeter = 3;
uint32_t microstep = 387.44*milimeter + 706.3; //12800; //8533; //25600;
uint8_t  n_turns   = 1;

//Contador de iteraciones de avance y contador de micropasos
volatile uint32_t cont      = 0;
volatile uint32_t cont_loop = 0;

//Variables para control de motor
const bool toEndPos_STATE       = true;  // true: counter-clockwise rotation
const bool toInitialPos_STATE   = false; // false: clockwise rotation

bool MOTOR_STATE = true;
volatile uint32_t microsteps_total = microstep*n_turns*2; 

//Variables auxiliares
//uint32_t      aux        = 0;
//volatile bool timer_flag = false; //Bandera para indicar paso por interrupcion

#define T_ESPERA_MS 2000

#define T_ESPERA_IS 50000

void setup()
{
    //Configuracion Serial y temporizador
    Serial.begin(115200);  
    configTimer1MicroSteps(); 
  
    //Pin Mode
    //Motor pins
    pinMode(STEP_Pin,OUTPUT);
    pinMode(DIR_Pin,OUTPUT);
    pinMode(EN_Pin,OUTPUT);
  
    pinMode(Lswitch,INPUT_PULLUP); //Limit switch: OPEN(NO DETECTED=1 AND DETECTED=0)
    pinMode(led,OUTPUT);  

    //Initial value
    digitalWrite(EN_Pin,HIGH);
    digitalWrite(DIR_Pin,toEndPos_STATE); // clockwise rotation
    digitalWrite(led,HIGH);
  
    //Posicion inicial
    //Serial.println("Bemvindo motor SAR");
    uint32_t marcaInicio = 0;
    marcaInicio = millis();
    //Serial.println("Inicio espera detenido");
    while(((uint32_t)(millis()-marcaInicio))<T_ESPERA_IS);
    
    end2InitialWay();

    
}

void loop(){
  
  initial2EndWay();
  end2InitialWay();
  
}

//With the settings above, this IRS will trigger each 1.875us.
ISR(TIMER1_COMPA_vect){
  TCNT1     = 0;         
  if (cont <= microsteps_total)
  {
    MOTOR_STATE = !MOTOR_STATE;          //Invert GENERATE MICROSTEP/LED state
    digitalWrite(STEP_Pin,MOTOR_STATE);  //Write new state to the GENERATE MICROSTEP/LED on pin D3 
    cont++;                              //Increase step_counter
  }
  
  //timer_flag =true;                      //Interrupt pass flag
}

void enaCompAInt(){
  /* We enable compare match mode on register A*/
  TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A 
}

void disCompAInt(){
  /*We disable compare match mode on register A*/
  TIMSK1 &= ~B00000010;       //Clear OCIE1A to 0 so we disable compare match A 
}

void cleanIntCompA(){
  TIFR1  |=  B00000010;       //Write OCF1A clean all pendent interruption for comparator
}

void configTimer1MicroSteps(){
  /*1. First we reset the control register to make sure we start with everything disabled.*/
  TCCR1A = 0;                 // Reset entire TCCR1A to 0 
  TCCR1B = 0;                 // Reset entire TCCR1B to 0
  /*2. We set the prescalar to the desired value by changing the CS10 CS11 and CS12 bits. */  
  TCCR1B |= B00000011;        //Set CS12 to 1 so we get prescalar 64  
  /*3. We enable compare match mode on register A*/
  //TIMSK1 |= B00000010;      //Set OCIE1A to 1 so we enable compare match A 
  /*4. Set the value of register A to 25*/
  OCR1A = 10;                //Finally we set compare register A to this value  
                             //Carga el registrador de comparación: ( 16MHz/(64*10) ) -1 = 533332 => 1.875 us
}

void unaSecuenciaIda(){
  while(cont<=microsteps_total);
  return;
}

void tiempoDetenido(){
   uint32_t marcaInicio = 0;
   marcaInicio = millis();
   //Serial.println("Inicio espera detenido");
   while(((uint32_t)(millis()-marcaInicio))<T_ESPERA_MS);
   //Serial.println("Fin espera detenido");
   return;
}

void initial2EndWay(){
  //Serial.println("Comienza movimiento de inicio a final");
  disCompAInt();
  cleanIntCompA();
  //Preparo contadores
  microsteps_total = microstep*n_turns*2;
  cont             = 0;
  cont_loop        = 0;
  digitalWrite(DIR_Pin, toEndPos_STATE); 
  enaCompAInt();
  while(cont_loop<173){
    Serial.println(5);
    Serial.println(6);
   // Serial.println(cont_loop);
    unaSecuenciaIda();
    tiempoDetenido();
    disCompAInt();
    cleanIntCompA();
    //Serial.print("Fin Vuelta: ");
    //Serial.println(cont_loop);
    //Preparo contadores
    cont = 0;
    cont_loop++;
    enaCompAInt();
  }   
  cont_loop = 0;
  //Serial.println("Fin de vueltas"); 
}

void end2InitialWay(){
  //Serial.println("Hacia inicio");
  Serial.println(7);
  Serial.println(8);
  disCompAInt();
  cleanIntCompA();
  //Preparo contadores
  microsteps_total = 1628661;
  cont             = 0;
  digitalWrite(DIR_Pin, toInitialPos_STATE); 
  enaCompAInt();
  while(digitalRead(Lswitch) == HIGH);
  disCompAInt();
  cleanIntCompA();
  //Serial.println("Llego a inicio");           
}
