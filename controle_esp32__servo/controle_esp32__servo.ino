#include <Wire.h> //Biblioteca que permite a comunicação I2C.
#include <Adafruit_PWMServoDriver.h> //Esta biblioteca deve ser baixada para rodar o código.
#include <WiFi.h>

const char* ssid = "*****";
const char* password = "******";

WiFiServer server(80);
 
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();//Instanciando objetos com a classe Adafruit_PWMServoDriver.
 
#define MIN_PULSE_WIDTH 650   //Estes são os valores mínimo e máximo de largura de pulso que servem para o MG 995.
#define MAX_PULSE_WIDTH 2350
#define DEFAULT_PULSE_WIDTH 1500
#define FREQUENCY 60
 
int pulseWidth(int angle){ //Esta função calcula o ângulo de movimento do servo.
int pulse_wide, analog_value;
pulse_wide = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH); //Esta função pega ângulo de 0 a 180 graus e mapeia do valor mínimo de largura até o valor máximo. 
analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
Serial.println(analog_value);
return analog_value; //O valor que a função retorna.
}
  
  
void setup(){
Serial.begin(115200);

  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
//Serial.println("16 channel Servo test!");
pwm.begin();  //Inicializa a biblioteca e envia sinais PWM.
pwm.setPWMFreq(FREQUENCY); //Frequência de atualização do servo a 60 Hertz.
pwm.setPWM(1,0,pulseWidth(0)); 
pwm.setPWM(2,0,pulseWidth(0)); 
pwm.setPWM(3,0,pulseWidth(0)); 
pwm.setPWM(4,0,pulseWidth(0));
pwm.setPWM(5,0,pulseWidth(0));
pwm.setPWM(6,0,pulseWidth(0));
}
 
void loop(){ 
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
         

            client.print("Click <a href=\"/H\">here</a> grupo B atuador 3<br>");
            client.print("Click <a href=\"/L\">here</a> grupo B atuador 4<br>");
            client.print("Click <a href=\"/G\">here</a> grupo A atuador 1<br>");
            client.print("Click <a href=\"/F\">here</a> grupo A atuador 2<br>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /H")) {
          pwm.setPWM(3,0,pulseWidth(90));
          delay(100);
          pwm.setPWM(3,0,pulseWidth(0));
         delay(100);
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
