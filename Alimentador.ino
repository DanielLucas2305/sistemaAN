const int tempo = 200; 

const int s10 = 50;
const int h8 = 144000;

void manual(){
  for (int i = 0; i < s10 ; i++){
    prog = digitalRead(pin_btn);
    delay(tempo);
    medirTemp();
//    prog = digitalRead(pin_btn);
    Serial.print("Funcao manual em  ");
    Serial.println(prog);
    if(i == 21 && !MQTT.connected()){
      conectaMQTT();
    }
    MQTT.publish(TPC_PBL_ALMTR, "Ligado Manualmente");
    if (prog == LOW){
      break;
    }
  }
}

void espera(){
  for (int i = 0; i < h8 ; i++){
    prog = digitalRead(pin_btn);
    if(i == 50 || i == 159 || i == 900 || i == 1599 || i == 50000 || i == 100000 || !MQTT.connected()){
      conectaMQTT();
    }
    if(i == 1 || i == 98 || i == 159 || i == 900 || i == 1599 || i == 50000 || i == 100000){MQTT.publish(TPC_PBL_ALMTR, "Em espera");};
    medirTemp();
    delay(tempo);
    Serial.print("Funcao espera em   ");
    Serial.println(prog);
    medirTemp();
    if (prog == HIGH){
      break;
    }
  }
}


void acionado(){
  for (int i = 0; i < s10 ; i++){
    prog = digitalRead(pin_btn);
    medirTemp();
    delay(tempo);
    if(i == 1 || i == 30 || i == 55){MQTT.publish(TPC_PBL_ALMTR, "Acionado");};
    Serial.print("Funcao acionado em    ");
    Serial.println(prog);
    medirTemp();
    if (prog == HIGH){
      break;
    }
  }
}
