// --------------- Bibliotecas e variáveis da Parte Gráfica -----------------------------------------------
#include <LiquidCrystal.h>
#include <math.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

int linha,coluna,pagina,toff,ton,npulsos,freq,ncan,a,b,c,d,e,k,result,n,micro,counter;
int lcd_key     = 0;
int adc_key_in  = 0;
int canais[] = {3, 11, 12, 13}; // Pinos de saida do sinal 

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      
 if (adc_key_in > 1000) return btnNONE; 
 if (adc_key_in < 80)   return btnRIGHT;
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;
 return btnNONE;  
}

int menu1(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" T.off  N.pulsos");
  lcd.setCursor(0,1);
  lcd.print(" T.on   T.eTrens");'
  lcd.setCursor(coluna,linha);
  lcd.print(">");
  }


int menu2(void){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" N.can  Executar");
  lcd.setCursor(0,1);
  lcd.print(" S.can  Teste");
  lcd.setCursor(coluna,linha);
  lcd.print(">");
  }

// --------------- Bibliotecas e variáveis do Giroscópio -----------------------------------------------
#include <MPU6050_tockn.h>
#include <Wire.h>
MPU6050 mpu6050(Wire);
int i,X_Relativo,Y_Relativo,Z_Relativo;
float novo_referencial_x,novo_referencial_y,novo_referencial_z;

// ---------------------------------------------------------------------------------------------------

// ------------------------- Bibliotecas e variaveis do Chip de memória ------------------------------
#include <Fat16.h>
#include <Fat16util.h> 

const uint8_t CHIP_SELECT = SS;  // SD chip select pin.
SdCard card;
Fat16 file;

// store error strings in flash to save RAM
#define error(s) error_P(PSTR(s))


void error_P(const char* str) {
  PgmPrint("Erro: ");
  SerialPrintln_P(str);
  if (card.errorCode) {
    PgmPrint("SD error: ");
    Serial.println(card.errorCode, HEX);
  }
  while(1);
}

//Funcao para escrever numero no SD 

void writeNumber(uint32_t n) {
  uint8_t buf[10];
  uint8_t i = 0;

  //Caso o numero seja negativo
  if (n>4290000000){
    n = 4294967295 - n;
    
    //Caso o numero seja 0
    if(n>0){
        file.write("-"); 
      }
    
    }
    
  do {
    i++;
    buf[sizeof(buf) - i] = n%10 + '0';
    n /= 10;
  } while (n);
  file.write(&buf[sizeof(buf) - i], i); // write the part of buf with the number
}
//-------------------------------------------------------------------------------------------------------------


void setup() {
// ------------------------------Setup do chip de memória ------------------------------------
  Serial.begin(115200);
  lcd.begin(16, 2);
  
//    // Inicializa o SD
 if (!card.begin(CHIP_SELECT)){
    lcd.setCursor(0,0);
    lcd.print("ERRO 1 - MicroSD");
    lcd.setCursor(0,1);
    lcd.print("Nao Identificado");
  
  error(" Inicialização do SD - ERRO 1"); // Chip micro SD não encontrado/inserido
 }
//  
//  // initialize o volume FAT16
  if (!Fat16::init(&card)) error(" Inicialização do SD - ERRO 2"); // Formatacao incorreta do SD (Deve ser feita em FAT 16)

 

  
  


// -------------------------- Setup das Saidas -------------------------------------------------   
   pinMode(3,OUTPUT);     // Setando saidas DESCOMENTAR
   pinMode(11,OUTPUT);
   pinMode(12,OUTPUT);
   pinMode(13,OUTPUT);
   
   digitalWrite(3,LOW);   // Setando saidas em nivel baixo
   digitalWrite(11,LOW);
   digitalWrite(12,LOW);
   digitalWrite(13,LOW);

// --------------------------------------------------------------------------------------------
// ------------------------------- Setup da parte Grafica -------------------------------------

   

    // Inicializa o SD
   if (!card.begin(CHIP_SELECT)) {
    lcd.setCursor(0,0);
    lcd.print("ERRO 1 - MicroSD");
    lcd.setCursor(0,1);
    lcd.print("Nao Identificado");
    error(" Inicialização do SD - ERRO 1"); // Chip micro SD não encontrado/inserido
    
   }
  
//   // initialize o volume FAT16
//   if (!Fat16::init(&card)){
//    lcd.setCursor(0,0);
//    lcd.print("ERRO 2 - MicroSD");
//    lcd.setCursor(0,1);
//    lcd.print(" Nao formatado ");
//    error(" Inicialização do SD - ERRO 2"); // Formatacao incorreta do SD (Deve ser feita em FAT 16)
//   }

  Serial.print("Inicialização do SD Realizada com sucesso!");
 


   lcd.setCursor(0,0);
   lcd.print("  Por Favor  ");
   lcd.setCursor(1,1);
   lcd.print("  Aguarde ....  ");
                 
   coluna = 0;
   linha = 0;
   pagina = 0;   
   a=0;
   toff=50;
   ton=50;
   npulsos=1;
   freq=10;
   ncan=1;
   k=0;
   n=0;
   
// --------------------------------------------------------------------------------------------
// -------------------- Setup do Giroscópio ---------------------------------------------
  
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  menu1();
}

void loop() {
  lcd.setCursor(0,1);          
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               
 {
   case btnRIGHT:
     {
        if(coluna == 0){
          lcd.setCursor(0,linha);
          lcd.print(" ");
          lcd.setCursor(7,linha);
          lcd.print(">");
          coluna=7;
          delay(300);
          }
     
     break;
     }
   case btnLEFT:
     {
         if(coluna == 7){
          lcd.setCursor(7,linha);
          lcd.print(" ");
          lcd.setCursor(0,linha);
          lcd.print(">");
          coluna=0;
          delay(300);
          }
     
     break;
     }
   case btnUP:
     {
        if(linha == 1){
          lcd.setCursor(coluna,1);
          lcd.print(" ");
          lcd.setCursor(coluna,0);
          lcd.print(">");
          linha=0;
          delay(300);
          }

      else if(linha == 0 and pagina == 1){
          linha=0;
          coluna=0;
          pagina=0;
          menu1();
          delay(300);
          }
     
     break;
     }
   case btnDOWN:
     {  if(linha == 0){
          lcd.setCursor(coluna,0);
          lcd.print(" ");
          lcd.setCursor(coluna,1);
          lcd.print(">");
          linha=1;
          delay(300);
          }
          
        else if(linha == 1 and pagina == 0){
          linha=0;
          coluna=0;
          menu2();
          pagina=1;
          delay(300);
          }
          
     
     break;
     }
   case btnSELECT:
     {
      // ----------------------------------------------- Menu toff -------------------------------------------------------------------------------
      if(linha == 0 and coluna == 0 and pagina == 0){
          lcd.clear();
           while (a==0) {  
                 lcd.setCursor(0,0);
                 lcd.print("Tempo Desligado");
                 lcd.setCursor(0,1);
                 lcd.print(toff);
                 lcd.print(" em us");
                    
                if (read_LCD_buttons() == btnUP and toff<200){
                  toff=toff+10;
                  delay(200);
                  }

                if (read_LCD_buttons() == btnDOWN and toff>50){
                  toff=toff-10;
                  delay(200);
                  }

                if (read_LCD_buttons() == btnLEFT ){
                  a=1;
                  delay(200);
                  }
            
            }
            menu1(); 
            a=0;
        }
  //-------------------------------------------------------------------------------------------------------------------------------------------------
   // ----------------------------------------------- Menu ton -------------------------------------------------------------------------------
    if(linha == 1 and coluna == 0 and pagina == 0){
          lcd.clear();
           while (a==0) {  
                 lcd.setCursor(0,0);
                 lcd.print("Tempo Ligado");
                 lcd.setCursor(0,1);
                 lcd.print(ton);
                 lcd.print(" em us");

                    
                if (read_LCD_buttons() == btnUP and ton<200){
                  ton=ton+10;
                  delay(200);
                  }

                if (read_LCD_buttons() == btnDOWN and ton>50){
                  ton=ton-10;
                  delay(200);
                  }

                if (read_LCD_buttons() == btnLEFT ){
                  a=1;
                  delay(200);
                  }
            
            }
            menu1(); 
            a=0;
        }
  //--------------------------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------- Menu do Numero de pulsos -------------------------------------------------------------------------------

  if(linha == 0 and coluna == 7 and pagina == 0){
          lcd.clear();
           while (a==0) {  
                 lcd.setCursor(0,0);
                 lcd.print("Numero de pulsos");
                 lcd.setCursor(0,1);
                 lcd.print(npulsos);
                 lcd.print(" pulsos");

                    
                if (read_LCD_buttons() == btnUP and npulsos<20){
                   npulsos= npulsos+1;
                  delay(200);
                  }

                if (read_LCD_buttons() == btnDOWN and npulsos>1){
                   npulsos= npulsos-1;
                  delay(200);
                  }

                if (read_LCD_buttons() == btnLEFT ){
                  a=1;
                  delay(200);
                  }

            }
            menu1(); 
            a=0;
        }

  //-------------------------------------------------------------------------------------------------------------------------------------------------
  // ----------------------------------------------- Menu da frequencia  -------------------------------------------------------------------------------
     if(linha == 1 and coluna == 7 and pagina == 0){
          lcd.clear();
           while (a==0) {  
                 lcd.setCursor(0,0);
                 lcd.print("T. entre trens");
                 lcd.setCursor(0,1);
                 lcd.print(freq);
                 lcd.print(" ms");

                    
                if (read_LCD_buttons() == btnUP and  freq<100){
                  freq=freq+10;
                  delay(200);
                  }

                if (read_LCD_buttons() == btnDOWN and  freq>10){
                    freq=freq-10;
                  delay(200);
                  }

                if (read_LCD_buttons() == btnLEFT ){
                  a=1;
                  delay(200);
                  }

            }
            menu1(); 
            a=0;
        }
 //------------------------------------------------------------------------------------------------------------------------------------------------- 
  //----------------------------------------------- Menu do numero de canais  -------------------------------------------------------------------------------
      if(linha == 0 and coluna == 0 and pagina == 1){
          lcd.clear();
           while (a==0) {  
                 lcd.setCursor(0,0);
                 lcd.print("Numero de canais");
                 lcd.setCursor(0,1);
                 lcd.print(ncan);
                 lcd.print(" canais");                    
                if (read_LCD_buttons() == btnUP and ncan<4){
                   ncan= ncan+1;
                  delay(200);
                  }
                if (read_LCD_buttons() == btnDOWN and ncan>1){
                   ncan= ncan-1;
                  delay(200);
                  }
                if (read_LCD_buttons() == btnLEFT ){
                  a=1;
                  delay(200);
                  }
            }
            linha=0;
            coluna=0;
            menu2(); 
            a=0;
      } 

 //------------------------------------------------------------------------------------------------------------------------------------------------- 
  //----------------------------------------------- Menu de selecao de canais  -------------------------------------------------------------------------------
      if(linha == 1 and coluna == 0 and pagina == 1){
         delay(200);
         b=0;
         c=0;
         d=0;
         e=0;
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("Sel. canais");
         lcd.setCursor(0,1);
         lcd.print("> 3  11  12  13");
         linha=1;
         coluna=0;
         lcd.setCursor(coluna,linha);
           while (a==0) {  

                if (read_LCD_buttons() == btnRIGHT and coluna < 11){
                   lcd.setCursor(coluna,linha);
                   lcd.print(" ");
                   coluna=coluna+3;
                if (coluna >= 6){
                   coluna=coluna+1;
                  }
                   lcd.setCursor(coluna,linha);
                   lcd.print(">");
                  delay(200);
                  }

                if (read_LCD_buttons() == btnLEFT and coluna > 0){
                   lcd.setCursor(coluna,linha);
                   lcd.print(" ");
                   coluna=coluna-4;
                   
                if (coluna < 3){
                   coluna=coluna+1;
                  }
                   lcd.setCursor(coluna,linha);
                   lcd.print(">");
                  delay(200);
                  }

                if (read_LCD_buttons() == btnSELECT ){ // canal b=3 c=11 d=12 e=13
                  if(coluna == 0){ // selecionou canal 3
                      b = 1;
                    }
                  if(coluna == 3){ // selecionou canal 11
                      c = 1;
                    }
                  if(coluna == 7){ // selecionou canal 12
                      d = 1;
                    }
                   if(coluna == 11){ // selecionou canal 13
                      e = 1;
                    }
                    lcd.setCursor(coluna+1,linha);
                    lcd.print("*");
                  }
                if((b+c+d+e) == ncan){
                   a=1;
                   delay(400);  
                   }
            }
            linha=0;
            coluna=0;
            menu2(); 
            a=0;
      } 

 //------------------------------------------------------------------------------------------------------------------------------------------------- 
 //----------------------------------------------- Menu de Execução  -------------------------------------------------------------------------------
    if(linha == 0 and coluna == 7 and pagina == 1){
        lcd.clear();
        linha=0;
        coluna=0;
        lcd.setCursor(coluna,linha);
        lcd.print("Aperte Esquerda");
        linha=1;
        coluna=0;
        lcd.setCursor(coluna,linha);
        lcd.print("para parar");
        delay(1500);
        lcd.clear();
        
        // Calibrando o sensor 
        linha=0;
        coluna=0;
        lcd.setCursor(coluna,linha);
        lcd.print("Calibrando...");
        linha=1;
        coluna=0;
        lcd.setCursor(coluna,linha);
        lcd.print("  Nao se mova  ");
        
        
        while (a==0) {

      
voltar:        while (k < npulsos ){
voltar2: // ----------------------- Acelerometro funcionando ------------------------- DESCOMENTAR

                      mpu6050.update();
      
                      // Esperar o sensor estabilizar para pegar a medicao de referencial
                      if (i == 200){
                          
                          mpu6050.update();
                          novo_referencial_x = mpu6050.getAngleX();
                          novo_referencial_y = mpu6050.getAngleY();
                          novo_referencial_z = mpu6050.getAngleZ();
                          lcd.clear();

                          // Inicio do Registro dos Dados no SD
                    
                          // Cria o nome do arquivo
                          char name[] = "Dados00.TXT";
                          for (uint8_t i = 0; i < 100; i++) {
                        
                            //Nome do arquivo de acordo com a execução do programa
                            name[5] = i/10 + '0';
                            name[6] = i%10 + '0';
                            
                            // O_CREAT - create the file if it does not exist
                            // O_EXCL - fail if the file exists
                            // O_WRITE - open for write
                            if (file.open(name, O_CREAT | O_EXCL | O_WRITE)) break;
                          }
                          
                          if (!file.isOpen()) error ("file.open"); // Caso de erro

                          Serial.println();
                          PgmPrint("Escrevendo em: "); // depois tirar
                          Serial.println(name);
                          
                          
                          // Escreva o que deve ser gravado no SD
                          //file.write("line "); // write string from RAM
                          //file.println(); 
  
                          
                          // Toff
                          file.write("Tempo off: ");
                          writeNumber(toff);
                          file.write(" us   ");
                          
                          //Ton
                          file.write("Tempo on: ");
                          writeNumber(ton);
                          file.write(" us   ");

                          //N pulsos
                          file.write("Numero de Pulsos: ");
                          writeNumber(npulsos);
                          file.write(" pulsos   ");

                          //T.entre Trens
                          file.write("Tempo entre Trens de Pulso: ");
                          writeNumber(freq);
                          file.write(" ms   ");

                          //Numero de canais selecionados
                          file.write("Canais Selecionados: ");
                          writeNumber(ncan);
                          file.write(" canais");
                          file.println();
                          file.write("X;Y;Z;");
                          file.println();
                          
                        }
              

            
                      if (i >=200){
                        X_Relativo = (int( mpu6050.getAngleX()- novo_referencial_x));
                        Y_Relativo = (int( mpu6050.getAngleY()- novo_referencial_y));
                        Z_Relativo = (int(mpu6050.getAngleZ() - novo_referencial_z));

                        
                        // Setando o display para mostrar o angulo
                        

                        lcd.setCursor(0,0);
                        lcd.print("Angulos  X: ");
                        lcd.setCursor(12,0);
                        lcd.print(X_Relativo);
                        lcd.print("  ");
                        lcd.setCursor(0,1);
                        lcd.print("Y: ");
                        lcd.print(Y_Relativo);
                        lcd.print("  ");
                        lcd.setCursor(9,1);
                        lcd.print("Z: ");
                        lcd.print(Z_Relativo);
                        lcd.print("  ");
                        lcd.setCursor(0,0);

                        //Escrevendo no SD
                        writeNumber(X_Relativo);
                        file.print(";");
                        writeNumber(Y_Relativo);
                        file.print(";");
                        writeNumber(Z_Relativo);
                        file.print(";");
                        file.println();
                      }
                      i++;

                  
      
//------------------------------------------------------------------------------  DESCOMENTAR
  
                  if ( b == 1){
                       digitalWrite(3,HIGH);
                    }
                  if ( c == 1){
                       digitalWrite(11,HIGH);
                    }
                  if ( d == 1){
                       digitalWrite(12,HIGH);
                    }
                  if ( e == 1){
                       digitalWrite(13,HIGH);
                    }
                  delayMicroseconds(ton); // tempo que os canais ficam ligados
                  
                  if ( b == 1){
                       digitalWrite(3,LOW);
                    }
                  if ( c == 1){
                       digitalWrite(11,LOW);
                    }
                  if ( d == 1){
                       digitalWrite(12,LOW);
                    }
                  if ( e == 1){
                       digitalWrite(13,LOW);
                    }

                     // incrementando o contador de pulsos
                     k=k+1;
                     result=1; // 150us
                     delayMicroseconds(toff); // 

                  if (read_LCD_buttons() == btnLEFT ){ // sair do menu
                        delay(200);
                        i = 0;
                        
                        // Fecha o Arquivo SD
                        file.close();
                        Serial.println("Operação de Escrita Terminada");
                        goto sai;
                        }
                        
                  if (k == npulsos){ // delay grande ms, depois de executar os N pulsos, dá o tempo grandão
                       delay(freq);
                       k=0;
                       goto voltar;
                    }

          
                  
                     
                  goto voltar2;
                  
               
            } 
                  
            } // while (a==0)
            
sai:            linha=0;
                coluna=0;
                menu2(); 
                a=0;
                k=0;
      
      }           

 //------------------------------------------------------------------------------------------------------------------------------------------------- 
 //----------------------------------------------- Menu de Teste -------------------------------------------------------------------------------

    if(linha == 1 and coluna == 7 and pagina == 1){
          lcd.clear();
           while (a==0) {  
                 lcd.setCursor(0,0);
                 lcd.print("Testando canal");
                 lcd.setCursor(0,1);
                 lcd.print("canal: ");
                 lcd.print(canais[n]);

                    
                if (read_LCD_buttons() == btnUP and n<3){
                   n=n+1;
                   lcd.setCursor(7,1);
                   lcd.print("   ");
                   lcd.setCursor(7,1);
                   lcd.print(canais[n]);
                   lcd.print(" ");
                  delay(200);
                  }

                if (read_LCD_buttons() == btnDOWN and n>0){
                   n=n-1;
                   lcd.setCursor(7,1);
                   lcd.print("  ");
                   lcd.setCursor(7,1);
                   lcd.print(canais[n]);
                  delay(200);
                  }

                if (read_LCD_buttons() == btnNONE ){
                   if (n == 0){ //testando canal 3
                      digitalWrite(3,HIGH);
                      delay(50);
                      digitalWrite(3,LOW);
                      delay(50);
                    }
                   if (n == 1){ //testando canal 11
                      digitalWrite(11,HIGH);
                      delay(50);
                      digitalWrite(11,LOW);
                      delay(50);
                    }
                    if (n == 2){ //testando canal 12
                      digitalWrite(12,HIGH);
                      delay(50);
                      digitalWrite(12,LOW);
                      delay(50);
                    }
                    if (n == 3){ //testando canal 13
                      digitalWrite(13,HIGH);
                      delay(50);
                      digitalWrite(13,LOW);
                      delay(50);
                    }
                  }
                if (read_LCD_buttons() == btnLEFT ){
                  a=1;
                  delay(200);
                  }
            }
            linha=0;
            coluna=0;
            menu2(); 
            a=0;
      }       
     break;
     }
     case btnNONE:
     {    
     break;
     }
 }
  

}
