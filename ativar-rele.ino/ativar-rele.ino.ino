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
  
  if(digitalRead(botao1)==HIGH){
    digitalWrite(rele1, HIGH);
  }else{
    digitalWrite(rele1, LOW);
  }  
}
