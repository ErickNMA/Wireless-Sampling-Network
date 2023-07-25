#include <SoftwareSerial.h>
//INCLUIR BIBLIOTECA
#include <LiquidCrystal.h>
//DIFINIR PINOS DO DISPLAY
LiquidCrystal lcd(12,11,4,5,6,7);

SoftwareSerial mySerial(2, 3); // RX, TX

int pinWrRd = 8;

bool writeOp = HIGH;
bool readOp = LOW;

void pisca() {
  digitalWrite(13, HIGH);
  delay(50); // Wait for 1000 millisecond(s)
  digitalWrite(13, LOW);
  delay(50); // Wait for 1000 millisecond(s)
}

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(pinWrRd, OUTPUT);
  digitalWrite(pinWrRd, writeOp );
  Serial.begin(9600);
  mySerial.begin(9600);  // UART 
  lcd.begin(16,2);
}

String datagrama( byte endereco, byte comando, byte io, byte valor ) {
  String pack = "" ;
  pack = pack + endereco;
  pack = pack + comando ; 
  pack = pack + io; 
  pack = pack + valor ;
  return pack;
}

String toHex2( byte aConv ) {
  byte bHi = aConv / 16;
  byte bLow = aConv & 0x0F ;
  String sHi = "";
  if (bHi <= 9) {
    sHi = sHi + bHi ;
  } else
  {
    sHi = sHi + (55 + bHi);
  }
  String sLow = "";
  if (bLow <= 9) {
    sLow = sLow + bLow ;
  } else
  {
    sLow = sLow + (55 + bLow);
  }
  return sHi + sLow;  
}


void transmiteComCS( String mensagem ) {
  byte CS = 0;
  for (int i=0; i < mensagem.length(); i++ ) {
    char cOne = mensagem.charAt(i) ;
    CS = CS xor cOne;
    Serial.print( cOne , HEX );
    delay(1);
    mySerial.print( cOne , HEX );
    delay(1);
  }

  String toTx = toHex2( CS );
  Serial.print( toTx );
  mySerial.print( toTx );
  delay(1);
}

int aguardaRead() {
    digitalWrite(pinWrRd, readOp );
    delay(1);
    unsigned long horaEnvio = millis();
    unsigned long passou = 0;
    String chegando = "";
    while (passou < 40) {
      passou = millis() - horaEnvio;
      if (mySerial.available() > 0) {
        char cc = mySerial.read();
        chegando = chegando + cc;
      }
    }
    //if (chegando.length() == 10) {
      // tamanho ok
    //}      
    //Serial.print("T");
    //int taman = chegando.length();
    //Serial.println( taman );
    lcd.clear();
    //DEFINE INICIO DO TEXTO
    lcd.setCursor(0,0);
    //ESCREVA 
    lcd.print("Slave 0");
    //DEFINE INICIO DO TEXTO SEGUNDA LINHA
    lcd.setCursor(2,1);
    lcd.print( chegando );
    Serial.println(chegando);

    // volta ao modo escrita
    digitalWrite(pinWrRd, writeOp );
    delay(1);
    while (mySerial.available()>0) {
      char cc = mySerial.read();
    }
}

String pacote = "";

void leituraSlave0() {
  // comando de leitura
  pacote = datagrama( 1 , 0 , 1 , 1 );
  transmiteComCS( pacote );     
  Serial.println("<>");
  // aguarda 20 ms a resposta
  aguardaRead();
}

void loop()
{

  leituraSlave0();
  //pisca();

  
  // Escrita, no endero 1 (slave1), 
  pacote = datagrama( 1 , 1 , 1 , 1 );
  transmiteComCS( pacote );   
  Serial.println(".");
  pisca();

  leituraSlave0();
  //pisca();

  
  pacote = datagrama( 2 , 1 , 1 , 1 );
  transmiteComCS( pacote );    
  Serial.println(".");
  pisca();


  leituraSlave0();
  //pisca();

  pacote = datagrama( 1 , 1 , 1 , 0 );
  transmiteComCS( pacote );   
  Serial.println(".");  
  pisca();

  leituraSlave0();
  //pisca();

  pacote = datagrama( 2 , 1 , 1 , 0 );
  transmiteComCS( pacote );    
  Serial.println(".");
  pisca(); 

}
