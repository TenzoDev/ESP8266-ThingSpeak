/* ------------Arduino+esp8266-01 thingSpeak example---------------------
 * --------Send data to Thingspeak channel using AT Command--------------
 * -----------------------------TENZO------------------------------------
*/
#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(2,3);      // arduino RX pin=2  arduino TX pin=3  

String apiKey = "Your-API-From-ThinSpeak";     // replace with your channel's thingspeak WRITE API key

String ssid="Your-WiFi-Name";    // Wifi network SSID
String password ="Your-WiFi-PassWord";  // Wifi network password

boolean DEBUG=true;

//----------- showResponce----------
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial.available()){
        c=espSerial.read();
        if (DEBUG) Serial.print(c);
      }
    }
                   
}

//-----Write in ThingSpeak------------
boolean thingSpeakWrite(float value1){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  espSerial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(espSerial.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  //getStr +="&field2=";
  //getStr += String(value2);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  espSerial.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(espSerial.find(">")){
    espSerial.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else{
    espSerial.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}

void setup() {
  pinMode(A1,INPUT);               
  DEBUG=true;           // enable debug serial
  Serial.begin(9600); 
  
  espSerial.begin(9600);  // enable software serial
                          // Your esp8266 module's speed is probably at 115200. 
                          // For this reason the first time set the speed to 115200 or to your esp8266 configured speed 
                          // and upload. Then change to 9600 and upload again
  
  //espSerial.println("AT+RST");         // Enable this line to reset the module;
  //showResponse(1000);

  //espSerial.println("AT+UART_CUR=9600,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
  //showResponse(1000);
  
  

  espSerial.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // connect to your WiFi
  showResponse(5000);

   if (DEBUG)  Serial.println("Setup completed");
}


void loop() {

  // Read sensor values
   float x = analogRead(A1);
   
        if (isnan(t)) {
        if (DEBUG) Serial.println("Failed to read from sensors");
      }
      else {
          if (DEBUG) Serial.println("Temp="+String(t)+" *C");
           thingSpeakWrite(x);                                      // Write values to thingspeak
      }
  
    
  // thingspeak needs 15 sec delay between updates,     
  delay(20000);  
}
