#include <WiFiManager.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <WebServer.h>
#include <DNSServer.h> //Local DNS Server used for redirecting all requests to the configuration portal ( https://github.com/zhouhan0126/DNSServer---esp32 )


//MQTT Server
const char* BROKER_MQTT = "test.mosquitto.org"; 
int BROKER_PORT = 1883;

WiFiClient wifiClient;
PubSubClient MQTT(wifiClient);

#define ID_MQTT  "projetoIntegrador3Termo"  //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define TPC_PBL_TEMP "t3mp374tu74A68a"
#define TPC_PBL_ALMTR "4l1m3nt4d0r"    

const int pin_led = 15;
const int pin_btn = 2;
int prog;


void setup() {
  pinMode(pin_led, OUTPUT);
  pinMode(pin_btn, INPUT);
  Serial.begin(115200);
  delay(2000);
  WiFiManager wifiManager;
  Serial.println("Sistema iniciando...");  
  delay(2500);
  wifiManager.resetSettings();
  //callback para quando entra em modo de configuração AP
  wifiManager.setAPCallback(configModeCallback); 
  //callback para quando se conecta em uma rede, ou seja, quando passa a trabalhar em modo estação
  wifiManager.setSaveConfigCallback(saveConfigCallback); 
  wifiManager.autoConnect("ESP_AP","12345678");
  delay(1000);
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  conectaMQTT();
//  prog = digitalRead(pin_btn);
  delay(1000);
}

void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}


void loop(){
  prog = digitalRead(pin_btn);
  if(!MQTT.connected()){
    conectaMQTT();
  }
  delay(500);
  Serial.print("Loop principal em  ");
  Serial.println(prog);
  if (prog == LOW){
    digitalWrite(pin_led, LOW);  // Desligar o relé
    espera();
//    delay(2000);
    digitalWrite(pin_led, HIGH);  // Ligar o relé 
    acionado();
  }
  else if (prog == HIGH){
    digitalWrite(pin_led, HIGH);  // Ligar o relé
//    delay(3000);
    manual();
  }
}




//callback que indica que o ESP entrou no modo AP
void configModeCallback (WiFiManager *myWiFiManager) {  
//  Serial.println("Entered config mode");
  Serial.println("Entrou no modo de configuração");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
  Serial.println(myWiFiManager->getConfigPortalSSID()); //imprime o SSID criado da rede

}

//callback que indica que salvamos uma nova rede para se conectar (modo estação)
void saveConfigCallback () {
//  Serial.println("Should save config");
  Serial.println("Configuração salva");
  Serial.println(WiFi.softAPIP()); //imprime o IP do AP
}
