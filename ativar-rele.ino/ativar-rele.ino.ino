/* Simples demostracao de ativacao de rele */

// indica que o rele1 eh ativado na porta 13
int rele1 = 13;
// indica que o botao1 eh conectado a porta 12
int botao1 = 12;

void setup() {
  pinMode(rele1, OUTPUT);
  pinMode(botao1, INPUT);
  

}

void loop() {
  //verifica o estado do pino do botao, deve estar no gnd ou 5v
  if(digitalRead(botao1)==HIGH){
  	// desliga 
    digitalWrite(rele1, HIGH);
  }else{
  	// liga
    digitalWrite(rele1, LOW);
  }  
}
