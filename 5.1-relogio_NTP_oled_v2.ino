//https://ntp.br/
//https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
//https://randomnerdtutorials.com/esp32-date-time-ntp-client-server-arduino/#more-95109
//https://www.youtube.com/watch?v=S9qDykVDJ-s
//https://www.manualdomaker.com/article/como-pegar-hora-da-internet-com-esp32-e-esp8266/
//https://blog.eletrogate.com/littlefs-alto-desempenho-para-rp-pico-esp32-e-esp8266/
//https://www.usinainfo.com.br/blog/esp32-projeto-relogio-oled-com-protocolo-ntp/
//https://www.youtube.com/watch?v=3cCetLjpWwY
//https://time.is/pt_br/compare/utc/S%C3%A3o%20Paulo
//https://curtocircuito.com.br/blog/protocolo-ntp-para-ESP32

//pino 22 do esp no sck do display
//pino 21 do esp no sda do display

/*
    estrutura de data e hora a partir do timeinfo
    %A - Nome completo do dia da semana
    %B - Nome completo do mês
    %d - Dia do mês
    %Y - Ano
    %H - Hora no formato 24h
    %EU - Hora no formato 12h
    %M - Minuto
    %S - Segundo 
 */ 

#include <Wire.h>
#include <WiFi.h>            //Biblioteca do WiFi
#include "time.h"            //inclusão a biblioteca time
#include <Adafruit_SSD1306.h>//inclusão da biblioteca do display oled

#define OLED_LARGURA 128     // Número de Pixels do display OLED (Largura)
#define OLED_ALTURA 64       // Número de Pixels do display OLED (Altura)
#define OLED_RESET   4       // pino de Reset
Adafruit_SSD1306 display(OLED_LARGURA, OLED_ALTURA, &Wire, OLED_RESET);//inicialização do display SSD1306

const char* ssid     = "FEM";    //Biblioteca do /FEM/WIFI_IOT_CFP117             
const char* password = "13@03@80@";       //Senha da rede ///13@03@80@/IoT117cFp
const char* ntpServer = "pool.ntp.org";  //servidor a ser acessado NTP
const long  gmtOffset_sec =-10800;       //compensação GMT: GMT do Brazil é com -3horas * 3600para converter em segundos assim = -10800// Deslocamento do horário de Greenwich (GMT) em segundos (fuso horário -3 horas)
const int   daylightOffset_sec =0;       //Deslocamento de horário de verão em segundos (nenhum neste caso)//mudar para 0 pois não é aplicavel no para o Brasil

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);//inicialização do display e endereçamendo I2C
  display.clearDisplay();                   // Limpa p display
  
  WiFi.begin(ssid, password);               //inicializa o WiFi para conectar a rede
      while (WiFi.status() != WL_CONNECTED) //Verifica o status do Wifi se está conectado ou não o teste é para verificar se não esta conectado manter-se no loop
      {
        display.clearDisplay();                  //Limpa p display
        display.setTextSize(1);                  //Tamanho da fonte
        display.setTextColor(SSD1306_WHITE);     //Cor da fonte
        display.setCursor(0,0);                  //coordenada coluna=1 e linha=8 para imprimir
        display.println("BUSCANDO REDE");        //texto
        Serial.print(".");                       //texto
        delay(500);                              //DELAY
        display.display();                       //finaliza a impressão das msg no display
      }                         //delay
        Serial.println("conectado");                      
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);// inicializa para obter o tempo
  printLocalTime();     //chama a subrotina para imprimir horario
  WiFi.disconnect(true);//desconecte o WiFi porque ele não é mais necessário desabilitar essa linha qdo associar alguma plataforma de IOT
  WiFi.mode(WIFI_OFF);  //desconecte o WiFi porque ele não é mais necessário
}

void loop()                                   
{                                           //inicio do programa principal
  printLocalTime();                                //chama a subrotina relogio
}                                           // fim do programa principal

void printLocalTime()                              //subrotina relogio
{                                           //inicio da subrotina
    struct tm timeinfo;                     //criação de uma struct tm chamada de timeinfo que contém todos os detalhes sobre o tempo (H:M:S DD/MM/AA)
    if(!getLocalTime(&timeinfo))            //Obtenha todos os detalhes sobre data e hora e salve na estrutura timeinfo
    {
     return;
     }                             
    display.clearDisplay();                  //Limpa p display
    display.setTextSize(1);                  //Tamanho da fonte
    display.setTextColor(SSD1306_WHITE);     //Cor da fonte
    display.setCursor(0,0);                  //coordenada coluna=1 e linha=8 para imprimir
    display.println("****** RELOGIO ******");//texto
    display.setTextSize(2);                  //Tamanho da fonte
    display.setCursor(15, 16);               //coordenada coluna=15 e linha=16
    display.println(&timeinfo, "%H:%M:%S");  //imprimi a struct timeinfo no formato Hora:Minuto:Segundo
    display.setTextSize(0);                  //Tamanho da fonte
    display.setCursor(30, 40);               //coordenada coluna=30 e linha=40
    display.println(&timeinfo, "%A");        //imprimi a struct timeinfo o dia da semana
    display.setCursor(20, 55);               //coordenada coluna=20 e linha=55
    display.println(&timeinfo, "%d/%B/%Y");  //imprimi a struct timeinfo no formato Dia/mes/ano
    display.display();                       //finaliza a impressão das msg no display
    delay(1000);                             //delay
 }                                           //fim da subrotina
