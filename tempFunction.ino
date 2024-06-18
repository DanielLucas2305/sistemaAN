//                                VIDA DE SILICIO
//                                    KIT ESP32
//                                      AULA 4 
//                            PROGRAMA 1 : LER SENSOR NTC10K 


int pinoNtc = 32;
double Vs = 3.3; // TENSÃO DE SAIDA DO ESP32
double R1 = 10000; //RESISTOR UTILIZADO NO DIVISOR DE TENSÃO
double Beta = 3950; // VALOR DE BETA
double To=298.15; // VALOR EM KELVIN REFERENTE A 25° CELSIUS
double Ro=10000;
double adcMax = 4095.0; 

double flag_temp;
  
void medirTemp(){
  int cont = 0;
  double adc = 0;
  if(!MQTT.connected()){
    conectaMQTT();
  }

  while(cont < 4){
    double Vout, Rt = 0;
    double T, Tc = 0;
    adc = analogRead(pinoNtc);// VARIÁVEL QUE RECEBE A LEITURA DO NTC10K
    //CALCULOS PARA CONVERSÃO DA LEITURA RECEBIDA PELO ESP32 EM TEMPERATURA EM °C
    Vout = adc * Vs/adcMax;
    Rt = R1 * Vout / (Vs - Vout);
    T = 1/(1/To + log(Rt/Ro)/Beta);
    Tc = T - 273.15;

    cont++;
    flag_temp += Tc;
    delay(100);
  }
  
  media();
  delay(200);   
}

void media(){
  if(!MQTT.connected()){
    conectaMQTT();
  }
  double med_temp = flag_temp / 4;
  String temCels = String(med_temp);
  MQTT.publish(TPC_PBL_TEMP, String(temCels).c_str());
//  Serial.println(temCels + " °C");
  flag_temp = 0;
}
