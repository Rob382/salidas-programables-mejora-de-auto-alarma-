void armadofn(){
//////////////////////////////ciclo para indicar el armadocon bocina/claxon//////////////////////////////////////////////////////// 
  if(sirena_activa == true){
    if(tonoactivado == false){
      // para cambiar la duración de silencios cambiar este valor |                       | este valor cambia el numero veces que va a sonar
      //                                                          |                       | el claxón (si es bocina dejar en 1)
      //                                                          |                       |
      //                                                          V                       V
      if ((tono == false) && ((currentMillis - ultimotonoact) >= 1000) && (contadortono < 1)){
      if(salida4_bocina == true){
      digitalWrite(5, HIGH);  
      }
      else{
      digitalWrite(2, HIGH);
      }
      tono = true;
      ultimotonoact = currentMillis;
      tiempo_sonido = 500;
      tiempo_silencio = 500;
      }
      // para cambiar la duración de tonos cambiar este valor    |                       | tiene que ser igual al de arriba siempre
      // (siempre es preferible cambiar este en lugar del de     |                       |
      // arriba)                                                 |                       |
      //                                                         V                       V
      if ((tono == true) && ((currentMillis - ultimotonoact) >= 1500) && (contadortono < 1)){
      if(salida4_bocina == true){
      digitalWrite(5, LOW);
      }
      else{
      digitalWrite(2, LOW);
      }
      tono = false;
      contadortono = contadortono+1;
      ultimotonoact = currentMillis;
      tiempo_sonido = 500;
      tiempo_silencio = 500;
      }
    }
  }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////acciones que toman las 4 salidas al armar/////////////////////////////////////////////////////////////
digitalWrite(0, salida_1_arm);
digitalWrite(1, salida_2_arm);
if (sirena_activa == false){
  digitalWrite(2, salida_3_arm);
}
if (salida4_activa == true && salida4_bocina == false){
  digitalWrite(5, salida_4_arm);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
}
