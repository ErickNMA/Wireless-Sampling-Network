// Inclusão das Bibliotecas
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial ESP(1, 4);      // RX pino 1 , TX pino 4

#define SAMPLES 30

// Endereco I2C do sensor MPU-6050
const int MPU = 0x68;

int **dataAlloc()
{
  int **M = (int **)calloc(SAMPLES, sizeof(int *));
  for (int i=0; i<SAMPLES; ++i)
  {
    M[i] = (int *)calloc(6, sizeof(int));
  }

  return M;
}

void dataFree(int **dados)
{
  for(int i=0; i<SAMPLES; i++)
    free(dados[i]);
  free(dados);
}

void dataUpdate(int **dados)
{
  for(int i=0; i<SAMPLES; i++)
  {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, (7*2), true); // Solicita os dados ao sensor
    for(int j=0; j<6; j++)
    {
      //Descarta valor de temperatura:
      if(j==3)
      {
        Wire.read();
        Wire.read();
      }
      dados[i][j] = ((Wire.read()<<8) + Wire.read());   
    }
    delay(100);
  }
}

void sendData(int **dados)
{
  for(int i=0; i<SAMPLES; i++)
  {
    for(int j=0; j<6; j++)
    {
      ESP.write((dados[i][j]+0x8000)>>8);
      ESP.write((dados[i][j]+0x8000)&0xff);
    }
  }
}

void setup() 
{
  // Inicializa Serial
  ESP.begin(9600);
  
  // Inicializa o MPU-6050
  Wire.begin();    // sda= 2 /scl= 0
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  // Configura Giroscópio para fundo de escala desejado
  /*
    Wire.write(0b00000000); // fundo de escala em +/-250°/s
    Wire.write(0b00001000); // fundo de escala em +/-500°/s
    Wire.write(0b00010000); // fundo de escala em +/-1000°/s
    Wire.write(0b00011000); // fundo de escala em +/-2000°/s
  */
  
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0b00000000);  // Trocar esse comando para fundo de escala desejado conforme acima
  Wire.endTransmission();
  
  // Configura Acelerometro para fundo de escala desejado
  /*
      Wire.write(0b00000000); // fundo de escala em +/-2g
      Wire.write(0b00001000); // fundo de escala em +/-4g
      Wire.write(0b00010000); // fundo de escala em +/-8g
      Wire.write(0b00011000); // fundo de escala em +/-16g
  */

    // Imprime na Serial os valores obtidos
  /* Alterar divisão conforme fundo de escala escolhido:
      Acelerômetro
      +/-2g = 16384
      +/-4g = 8192
      +/-8g = 4096
      +/-16g = 2048

      Giroscópio
      +/-250°/s = 131
      +/-500°/s = 65.6
      +/-1000°/s = 32.8
      +/-2000°/s = 16.4
  */
  
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00001000);  // Trocar esse comando para fundo de escala desejado conforme acima
  Wire.endTransmission();
}

void loop() 
{
  int **data = dataAlloc();
  //dataUpdate(data);
  data[0][0] = 8192;
  sendData(data);
  dataFree(data);
  delay(550);
}