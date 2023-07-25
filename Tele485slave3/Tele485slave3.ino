#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

int myAdd = 0;

String buf = "";
int stRec = 0;
unsigned long lastRec=0;
unsigned long tOcioso=0;

void setup()
{
  pinMode(13, OUTPUT);
  mySerial.begin( 9600 );
  Serial.begin( 9600 );
  pinMode(12 , INPUT_PULLUP );
  pinMode(11 , INPUT_PULLUP );
  
  int valIn1 = digitalRead( 12 );
  int valIn2 = digitalRead( 11 );
  myAdd = valIn1*2 + valIn2 ; 

  Serial.print("ADD " );
  Serial.println( myAdd );
  
}


// Guardando o pacote
String pacote = "";

byte ordChar( char carac ) {
  byte numOrdChar = 0;
  if ( (carac >= 'A') && (carac <= 'F') ) {
    numOrdChar = carac - 65 + 10;
  } else
  if ( (carac >= 'a') && (carac <= 'f') ) {
    numOrdChar = carac - 97 + 10;
  } else
  if ( (carac >= '0') && (carac <= '9') ) { 
    numOrdChar = carac-48;    
  }
  return( numOrdChar );
}

String converteHex( String hexPac ) {
  int tam = hexPac.length();
  String convertida = "";
  for (int i=0; i < tam; i=i+2 ) {
    char cHi  = hexPac.charAt( i );
    char cLow = hexPac.charAt( i+1 );
    char cc = ordChar(cHi) * 16 + ordChar(cLow);
    convertida = convertida + (char) cc;
  }
  return convertida;
}


int confereCS(){
  String mensagem=converteHex(pacote);
  int tam = mensagem.length();
  Serial.println(" cs processing ");
  Serial.println( mensagem );
  
  byte CS = 0;
  for (int i=0; i < (tam-1); i++ ) {
    char cOne = mensagem.charAt(i) ;
    CS = CS xor cOne;
    //Serial.print( cOne , HEX );
    //Serial.println( CS, HEX );
  }
  byte originalCS = mensagem.charAt(tam-1);
  int conferiu = originalCS == CS;
  return conferiu;
}

void loop()
{
  while (mySerial.available() > 0) {
    char cIn = mySerial.read();
    delay(1);
    Serial.write( cIn );
    delay(1);
    lastRec = millis();
    stRec = 1;
    pacote = pacote + cIn;
  }
  
  if (stRec==1) {
    tOcioso = millis()-lastRec;
    if (tOcioso > 10) {
      stRec=0;
      if (confereCS()) { 
        Serial.println("!");

        Serial.print(">");
        Serial.println(pacote);
        int endereco = ordChar(pacote.charAt(0))*16 + ordChar(pacote.charAt(1)) - 48;
        int comando = ordChar(pacote.charAt(2))*16 + ordChar(pacote.charAt(3)) - 48;
        int io = ordChar(pacote.charAt(4))*16 + ordChar(pacote.charAt(5)) - 48;
        int valor = ordChar(pacote.charAt(6))*16 + ordChar(pacote.charAt(7)) - 48;
        
        Serial.print(">> ");
        Serial.print( endereco ); Serial.print(" ");
        Serial.print( comando ); Serial.print(" ");
        Serial.print( io ); Serial.print(" ");
        Serial.print( valor ); Serial.print(" ");
        Serial.println();
 
        if (endereco == myAdd) {
          Serial.println("SOU EU!!");
          if (comando==1) {
            digitalWrite( 13 , valor ); 
          }
        }
      
      } else
      { Serial.println("?");
      }
      pacote = "";
      
    }
  }
}
