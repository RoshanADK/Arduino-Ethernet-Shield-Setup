#include <Ethernet.h>
#include <SPI.h>
//network NB: Pins 10, 11, 12 and 13 are reserved for Ethernet module. 
byte mac[] = { 0xDE, 0xDD, 0xBE, 0xEF, 0xEE, 0xED };
byte ip[] = { YOUR IP HERE };
byte gateway[] = { YOUR GATEWAY HERE};
byte subnet[] = { YOUR SUBNET HERE };

String inString = String(35);

String Led;
String Led1;

int Light[] = {00, 2}; //Led pins num 0 in arry is not used
int numofleds =1; //numofleds

int Fan[] = {00, 3}; //Led pins num 0 in arry is not used
int numofleds1 =1; //numofleds

String value[] = {"on","on","on","on","on","on","on","on","on"}; //startup all led are off
String value1[] = {"on","on","on","on","on","on","on","on","on"}; //startup all led are off

EthernetServer server(50);
String data;
void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip,gateway,subnet); 
  server.begin();
 //set pin mode
  for (int j = 1; j < (numofleds + 2); j++){
    pinMode(Light[j], OUTPUT);
  }
    for (int j = 1; j < (numofleds1 + 2); j++){
    pinMode(Fan[j], OUTPUT);
  }
  Serial.println("Serial READY");
  Serial.println("Ethernet READY");
  Serial.println("Server READY");
}

void loop()
{
  EthernetClient client = server.available();
  
  if(client){
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
     
      if(client.available()) {
      
        char c = client.read();
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (inString.length() < 35) {
            inString.concat(c);
         } 
        if (c == '\n' && current_line_is_blank) {
                    
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html><body><form method=get>");
          client.println("<p>IOT BASED HOME AUTOMATION</p>");
         int i=1;
        // for(int i=1;i < (numofleds + 1) ;i++){ 
           Led = String("Light") + i;
           Led1 = String("Fan") + i;
           
          if(inString.indexOf(Led+"=on")>0 || inString.indexOf("all=on")>0){
            Serial.println(Led+"on");
            digitalWrite(Light[i], HIGH);
            value[i] = "off"; 
          }
         else if(inString.indexOf(Led+"=off")>0 || inString.indexOf("all=off")>0 ){          
            Serial.println(Led+"on");
            digitalWrite(Light[i], LOW);
            value[i] = "on";
          }
          else if(inString.indexOf(Led1+"=on")>0 || inString.indexOf("all=on")>0){
            Serial.println(Led1+"on");
            digitalWrite(Fan[i], HIGH);
            value1[i] = "off"; 
          }
            else if(inString.indexOf(Led1+"=off")>0 || inString.indexOf("all=off")>0 ){          
            Serial.println(Led1+"on");
            digitalWrite(Fan[i], LOW);
            value1[i] = "on";
          }
           client.println("<br>"+Led+"  <input type=submit name="+Led+" value="+value[i]+">");
           client.println("<br>"+Led1+"  <input type=submit name="+Led1+" value="+value1[i]+">");
        // }
         client.println("<br>All <input type=submit name=all value=on><input type=submit name=all value=off>");
         client.println("</from></html></body>");
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    inString = "";
    client.stop();
  }
}
