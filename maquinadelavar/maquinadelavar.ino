/*
  Programa de controle para maquina de lavar
  Paulo Marcos Trentin
  em: 04-10-11
  terminado versao 1.0 as 16:26 - coisas a melhorar
*/
  
#include <EEPROM.h>
#include <LiquidCrystal.h>
  
//  Controle dos pinos atuadores
#define pinA1 A4
#define pinA2 4
#define pinA3 3
#define pinA4 5
#define pinB1 7
#define pinB2 A2
#define pinB3 A3
#define pinC1 A0
#define pinC2 6
#define pinD1 A1
#define pushButton 2
  
//  Dados para controle da EEPROM
#define WORK_STATE 0
//  A = 0, B = 1, C = 2, D = 3, E = 4, F = 5
#define PROGRAM_LETTER 1
//  Controle de reinicio. Usado para exibir mensagens
#define REBOOT 2
  
//  Define variaveis globais
int letter = 0; //A
  
//  Matriz da programacao da maquina (matriz de 10x60)
boolean states[][60] =  {   
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0},   
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,0,0,1,1,1,1,1,1,1,1,1,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1},
{0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
};
  
//  Define sequencia de estagios para cada letra (matriz de 6x2)
int letters[][2] = { {1,15},  //A
                      {16,29}, //B
                      {30,37}, //C
                      {38,41}, //D
                      {42,50}, //E
                      {51,60}  //F
                    };
/*
  min 42 e 43 esvazia agua do cesto
  44 centrifuga
  47 enche de agua
  
/*
/*  Obtem a letra de acordo com seu valor decimal */
char* getLetter()
{
  byte letter = getState();
  
  for (int i = 0; i < 6; i++){       if (letter >= letters[i][0] && letter <= letters[i][1]){
        return getCharLetter(i);
      }
  }
}
  
char* getCharLetter(int i)
{
  switch(i){
    case 0:
      return("A");
    case 1:
      return("B");
    case 2:
      return("C");
    case 3:
      return("D");
    case 4:
      return("E");
    case 5:
      return("F");
  }
}
  
//int vectorPins[] = {A4,3,4,5,6,7, A0, A1, A2, A3};
int vectorPins[] = {pinA1, pinA2, pinA3, pinA4, pinB1, pinB2, pinB3, pinC1, pinC2, pinD1};
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int pins = 10;
  
void setup() {
  Serial.begin(9600);
  
  //  Inicia LCD
  lcd.begin(16, 2);
  
  //  Define pinos como saida
  for (int i = 0; i < pins; i++){     
    pinMode(vectorPins[i], OUTPUT);   
  }   
   
  //  Usada em manutencao para inicio da maquina   
  //eraseEEPROM();   
   
    //  Verifica estado atual   
    int state = getState();   
      if (state == 60){     
        //  Imprime mensagem de boas vindas     
        printStartupMessage();     
        //  Deixa a maquina em estado de descanso (passo 60)     
        setPins(60);   
      }else{     
        int value = EEPROM.read(REBOOT);     
        Serial.print("eemprom value: ");     
        Serial.println(value);     
        if (value == 0){       
          for (int i = 5; i > 0; i--){
            lcd.clear();
            lcd.print("Voltando ao");
            lcd.setCursor(0,1);
            lcd.print("trabalho em ");
            lcd.print(i);
            lcd.print("...");
            delay(1000);
      
            if (checkButton()){
              delay(500);
              if (!digitalRead(pushButton)){
                setState(60);
                lcd.clear();
                lcd.print("Trabalho");
                lcd.setCursor(0,1);
                lcd.print("cancelado");
                delay(2000);
                break;
              }
            }
      }
    }else{
      //  Desativa flag
      EEPROM.write(REBOOT, 0);
    }
  
    boolean control = true;
    if(getState() == 60){
      control = false;
    }else{
      lcd.clear();
      lcd.print("Vamos la!");
      delay(1500);
    }
  
    while(control){
      control = newStep();
      //  Aguarda 2 minutos
      Serial.print("Aguardando 120 segundos: ");
      for (int i = 1; i <= 120; i++){         delay(1000);         Serial.print(i);         Serial.print(" ");       }       Serial.println("Ok");     }   }   boolean control = true;   while(control){     if (checkButton()){       delay(500);       if (!digitalRead(pushButton)){         delay(500);         if (!digitalRead(pushButton)){           //  Informa ao usuario o inicio da rotina           lcd.clear();           lcd.print("Iniciando");           lcd.setCursor(0,1);           lcd.print("letra ");           lcd.print(getCharLetter(letter -1));           lcd.print(" ...");           Serial.println("Clicou > 1 segundo");
  
          //  Executa o inicio da rotina
          setState(letters[letter -1][0]);
          delay(2000);
  
          //  Reinicia rotina do programa para rodar a configuracao
          EEPROM.write(REBOOT, 1);
          delay(1000);
          asm volatile ("jmp 0x0000");
        }
      }else{
        //  Controla o tamanho de letter
        if (letter == 6) letter = 0;
  
        lcd.clear();
        lcd.print("Rotina ");
        lcd.print(getCharLetter(letter));
  
        //  Incrementa letter
        letter++;
      }
      Serial.print("Clicou");
    }
  }
}
  
void printStartupMessage()
{
  //  Verifica se a maquina estava lavando algo
  lcd.print("Oi Neusa, vamos");
  lcd.setCursor(0,1);
  lcd.print("lavar?");
}
  
/*
  Manipula botao de interacao com o usuario
*/
boolean checkButton()
{
  int value = digitalRead(pushButton);
  
  if (value == 0){
    Serial.println("Clicou");
  
    //  Apenas faz beep
    beep();
    return true;
  }
  
  return false;
}
  
/*
  Zera a EEPROM do Atmega8/168 (512 Bytes)
*/
void eraseEEPROM()
{
  for (byte i = 0; i < 255; i++){     
  EEPROM.write(i, 0);   
}  
 
//  Inicia maquina desligada (estado = 60)   
EEPROM.write(WORK_STATE, 60);   
  //  Define letra A como ativa   
  EEPROM.write(PROGRAM_LETTER, 0);   
  //  Ajusta flag   
  EEPROM.write(REBOOT, 0); 
} 
/*   Salva estado atual do programa na EEPROM.   O estado e armazenado em passos, 1 - 59 no primeiro endereço da memoria (0) */
void setState(byte value){   
  Serial.print("Salvo estado WORK_STATE: ");   
  Serial.println(value, BIN);   
  EEPROM.write(WORK_STATE, value); 
} 
 
/*   Le o estado atual do programa */
byte getState(){   
  return EEPROM.read(WORK_STATE); 
} 
 
/*   Avanca um passo no programa */
boolean newStep() {   
  //  Obtem o passo atual   
  int state = getState();   
  //  Exibe no lcd o que esta fazendo   
  lcd.clear();   
  lcd.print("Executando");   
  lcd.setCursor(0,1);   
  lcd.print("tarefa ");   
  lcd.print(getLetter());   
  lcd.print("  Min:");   
  lcd.print(state);   
  if (state >= 60){
    //  Finaliza programa
    Serial.println("Programa encerrado.");
    lcd.clear();
    lcd.print("Uffa,");
    lcd.setCursor(0,1);
    lcd.print("       terminei!");
    setPins(60);
  
    delay(1000);
  
    delay(500);
    beep();
    delay(200);
    beep();
    delay(200);
    beep();
  
    return false;
  }
  
  //  Executa fisicamente o passo (aciona reles)
  setPins(state);
  
  //  Se o passo for 1 ou 47, atrasa 5 minutos
  if (state == 1 || state == 47){
    Serial.print("Atrasando 5 minutos para encher de agua: ");
  
    for (int i = 1; i <= 300; i++){
      Serial.print(i);
      Serial.print(" ");
      delay(1000);
    }
    Serial.println(" Ok");
  }
  
  //  Avanca e salva o passo na memoria
  Serial.print("State: ");
  Serial.println(state);
  setState(++state);
  
  return true;
}
  
void setPins(int state){
  for (int j = 0; j < 10; j++){
    boolean value = states[j][state -1];
    Serial.print(value,BIN);
    digitalWrite(vectorPins[j], value);
  }
  Serial.println();
}
  
void beep() {tone(A5, 900, 100);}
  
void loop() {}
