#include <VirtualWire.h>
#include <DHT.h>

#define DHTPIN 7
#define DHTTYPE DHT22   // Sensor DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);

char Valor_CharMsg[4]; 
//Armazena estado led = ligar/desligar
int estado = 0;

void setup() 
{
  Serial.begin(9600);   
  //Pino ligado ao pino DATA do transmissor RF
  vw_set_tx_pin(4);
  //Velocidade de comunicacao (bits por segundo)
  vw_setup(2000);
  Serial.println("Trasmissao modulo RF ");
  dht.begin();
  
}

void loop() 
{
  // Temperatura 
  delay(2000);
  float h = dht.readHumidity();
  // Leitura da temperatura (Celsius)
  float t = dht.readTemperature();
  // Verifica se o sensor esta respondendo
  if (isnan(h) || isnan(t))
  {
    Serial.println("Falha ao ler dados do sensor DHT !!!");
    return;
  }

//   ################################
    char charV[50];
    //dtostrf(t,4,4,charV);
    String teste= "Temp: "+String(t)+" C "+"Umidade: "+String(h)+" %";
//    charV= teste;
  //  teste.toCharArray(charV, strlen(teste));
    
    teste.toCharArray(charV,50);
    const char *msg = charV;
    
    //const char *msg = "hello";

    //Envio dos dados
    vw_send((uint8_t *)msg, strlen(msg));
    //Aguarda envio dos dados
    vw_wait_tx();
    
    // Mostra a temperatura no serial monitor e no display
    Serial.print("Temperatura: "); 
    Serial.print(t);
    Serial.print(" *C  ");
    Serial.print("Umidade : "); 
    Serial.print(h);
    Serial.println(" %");
      
    delay(3000);
  
}
