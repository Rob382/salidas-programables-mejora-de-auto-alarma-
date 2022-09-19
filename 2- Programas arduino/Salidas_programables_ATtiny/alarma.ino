void alarmafn(){
if(sirena_activa == true){
  if(tonoactivado == false){

//////////////////////////////ciclo que alterna sonido y silencio de bocina/claxon//////////////////////////////////////////////////////////
    if ((tono == false) && ((currentMillis - ultimotonoact) >= tiempo_silencio) && (desarmado == false)){
    digitalWrite(2, HIGH);
    digitalWrite(0, HIGH);
    tono = true;
    ultimotonoact = currentMillis;
    }
    if ((tono == true) && ((currentMillis - ultimotonoact) >= tiempo_sonido) && (desarmado == false)){
    digitalWrite(2, LOW);
    digitalWrite(0, LOW);
    tono = false;
    contadortono = contadortono+1;
    ultimotonoact = currentMillis;
    }
    if (desarmado == true){
      digitalWrite(2, LOW);
    }

    if(contadortono >= 3){                                      //esta sección es para cambiar la duración de tiempos de sonido y silencio
      tiempo_silencio = 1000;                                   //para generar el patrón de SOS en clave morse
    }
    if(contadortono >= 4){                                      //
      tiempo_silencio = 500;                                    //
    }    
    if(contadortono >= 6){                                      //
      tiempo_sonido = 1000;                                     //
      if (contadortono >= 9){                                   //
        contadortono = 0;                                       //
        tiempo_sonido =500;                                     //
      }
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////acciones que toman las 4 salidas al entrar en estado de alarma/////////////////////////////////////////////////
  if (notif_activo == false){
  digitalWrite(0, salida_1_alrm);
  }
  
  digitalWrite(1, salida_2_alrm);
  
  if (sirena_activa == false){
    digitalWrite(2, salida_3_alrm);
  }
  if (salida4_activa == true && salida4_bocina == false){
    digitalWrite(5, salida_4_alrm);
  }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
