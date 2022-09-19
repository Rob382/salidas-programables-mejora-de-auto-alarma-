//D5 = relay 4 
//D2 = relay 3
//D1 = relay 2 
//D0 = relay 1
//D3 = ¡input 1
//D4 = !input 2
////////////////////////////////////general////////////////////////////////////////
byte numero_de_entradas = 2;              //una entrada en modo notificador o dos en modo notif y sirena
const byte notif_activo = true;           //activar o desactivar notificador en relay 1 (normalmente cerrado)
const byte sirena_activa = true;          //activar o desactivar sirena en el relay 3 (normalmente abierto)
const byte salida4_activa = false;        //si se activa se deben reprogramar los fusibles del attiny con el programador de alto voltaje
const byte salida4_bocina = true;        //si se activa se va a utilizar para la bocina, se deben programar los fusibles ¡no pueden estar las dos en true al mismo tiempo!
////////////////////////////////////armado////////////////////////////////////////
const byte salida_1_arm = HIGH;
const byte salida_2_arm = HIGH;
const byte salida_3_arm = LOW;              //solo disponible si se desactiva la sirena con la variable "sirena_activa"
const byte salida_4_arm = LOW;              //solo disponible programando fusibles
///////////////////////////////////desarmado//////////////////////////////////////
const byte salida_1_dis = LOW;
const byte salida_2_dis = LOW;
const byte salida_3_dis = LOW;              //solo disponible si se desactiva la sirena con la variable "sirena_activa"
const byte salida_4_dis = HIGH;              //solo disponible programando fusibles
///////////////////////////////////alarma/////////////////////////////////////////
const byte salida_1_alrm = HIGH;
const byte salida_2_alrm = HIGH;
const byte salida_3_alrm = LOW;              //solo disponible si se desactiva la sirena con la variable "sirena_activa"
const byte salida_4_alrm = LOW;              //solo disponible programando fusibles
//////////////////////////////////////////////////////////////////////////////////

byte armado = false;
byte desarmado = false;
byte alarma = false;
unsigned long currentMillis = 0;
unsigned long ultimaentrada1 = 0;
unsigned long ultimaentrada2 = 0;
byte tonoactivado = false;
byte tono = false;
unsigned long ultimotonoact;
byte contadortono = 0;
int tiempo_sonido = 500;
int tiempo_silencio = 500;
    
/////////////// una entrada///////////////////////
byte entrada_1_prev = 0;
unsigned long duracion_0_logico = 0;
unsigned long duracion_1_logico = 0;
unsigned long logico_0_largo = 0;
unsigned long logico_1_largo = 0;
unsigned long pulsacion_previa = 0;
byte logico_0_pulso_largo_prev = false;
byte logico_1_pulso_largo_prev = false;
byte contador_pulsaciones = 0;
byte sirena_activada_por_pulsos = 0;
//////////////////////////////////////////////////
void setup() {
pinMode (3, INPUT);
pinMode (4, INPUT);
pinMode (0, OUTPUT);
pinMode (1, OUTPUT);
pinMode (2, OUTPUT);
pinMode (5, OUTPUT);

delay(2000);
}

void loop() {
currentMillis = millis();
if (numero_de_entradas == 1){
 leer_1_entrada(); 
}
if (numero_de_entradas == 2){
 leer_2_entradas();
}
if (armado == true){
  armadofn();
}
if (desarmado == true){
  desarmadofn();
}
if (alarma == true){
  alarmafn();
}

}
