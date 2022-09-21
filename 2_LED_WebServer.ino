#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid     = "Rodrigo";
const char* password = "password";

WiFiServer server(80);

//Se tiene un LED incorporado en terminal 15, se puede usar otro GPIO
#define LED  12
String estado = "";

void setup() 
{
    Serial.begin(115200);
    pinMode(LED, OUTPUT); // Se configura GPIO 15 como output
    digitalWrite(LED, LOW); // Manda un 0 (LED apagado)

    // Conectando a WiFi
    WiFi.begin(ssid, password);
    
    // Checa si esta conectado
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.print("La dirección IP es: ");
    Serial.println(WiFi.localIP());  //Muestra IP
    // Inicio del Servidor web.
    server.begin();
    Serial.println("Servidor web iniciado.");
}  
  
void loop() 
{
    // Consulta si se ha conectado algún cliente.
    WiFiClient client = server.available();
    if (!client) 
    {
      return;
    }

    Serial.print("Nuevo cliente: ");
    Serial.println(client.remoteIP());

    // Espera hasta que el cliente envíe datos.
    while(!client.available())
    { 
      delay(1); 
    }

    //////////////////////////////////////////////
    // Lee la información enviada por el cliente.
    String req = client.readStringUntil('\r');
    Serial.println(req);

    // Realiza la petición del cliente.
    if (req.indexOf("on2") != -1) 
    {
        digitalWrite(LED, HIGH); estado = "Encendido";
    }
    if (req.indexOf("off2") != -1)
    {
        digitalWrite(LED, LOW); estado = "Apagado";
    }
    
    //////////////////////////////////////////////
    // Página WEB. ////////////////////////////
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  Importante.
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><meta charset=utf-8></head>");
    client.println("<body><center><font face='Arial'>");
    client.println("<h1>Servidor web con ESP32.</h1>");
    client.println("<h2><font color='#00FF00'>Edgar Lara</h2>");
    client.println("<h2><font color='#00FF00'>Anmol Pathry</h2>");
    client.println("<h2><font color='#00FF00'>Jaime Gutiérrez</h2>");
    client.println("<h2><font color='#00FF00'>Rodrigo Zamora</h2>");
    client.println("<h2><font color='#0000FF'>Webserver 1.0</font></h2>");
    client.println("<h3>Práctica boton</h3>");
    client.println("<img src='https://c.tenor.com/5FAYuypOiKcAAAAC/cat-please.gif' height='200'/>");
    client.println("<h1 style=""></h1>");
    client.println("<div id='By' style='position:absolute; left:25%; top:370px; width:210px; height:210px;'>");
    client.println("<br><br>");
    client.println("<a href='on2'><button>Enciende LED</button></a>");
    client.println("<a href='off2'><button>Apaga LED</button></a>");
    client.println("<br><br>");
    client.println(estado);
    client.println("</font></center></body></html>");
    Serial.print("Cliente desconectado: ");
    Serial.println(client.remoteIP());
    client.flush();
    client.stop();
}  
  
