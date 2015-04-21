/*
 mc-typ-13 ver :1.0
*/
#include <MsTimer2.h>
#include <SPI.h>
#include <Ethernet.h>

/*device  */
String MC_ID ="1";
String mAppKey="";
String mRestKey="";

char server[] = "agri-kuc.appspot.com";  //DNS

#define mSecHttp 300000

#define SENSOR   0
#define SENSOR_1 1
#define SENSOR_2 2
#define SENSOR_3 3

/* http */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress ip(192,168,1,200 );
EthernetClient client;

boolean mNOW_CONE=false;
String mResponse3 ="";

void timer_func() {
    mResponse3="";
    proc_start();
}

void proc_start() {
  int iSen =0;
  int iSen2=0;
  int iSen3=0;
  int iSen4=0;
  
  iSen =analogRead(SENSOR);
  iSen2 =analogRead(SENSOR_1);
  iSen3 =analogRead(SENSOR_2);
  iSen4 =analogRead(SENSOR_3);
  
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);  
  String sSen  = String(iSen);
  String sSen2  = String(iSen2);
  String sSen3  = String(iSen3);
  String sSen4  = String(iSen4);
  
  //MC_ID
  Serial.println("===== request =====");
  Serial.println("SEN_1=" + sSen );

  Serial.println("connecting...");
  String sHost ="Host: ";
  sHost.concat(server);  
  if (client.connect(server, 80)) {
    Serial.println("connected");
    String sReq= "GET /trans.php?mc_id=";
    sReq.concat(MC_ID);
    sReq.concat("&rkey=");
    sReq.concat(mRestKey);
    sReq.concat("&apkey=");
    sReq.concat(mAppKey);
    sReq.concat("&snum_1=");
    sReq.concat(sSen);
    sReq.concat("&snum_2=");
    sReq.concat(sSen2);
    sReq.concat("&snum_3=");
    sReq.concat(sSen3);
    sReq.concat("&snum_4=");
    sReq.concat(sSen4);
    sReq.concat(" HTTP/1.1");
Serial.println(sReq);    
    client.println(sReq);
    client.println( sHost );
    client.println("Connection: close");
    client.println();
    mNOW_CONE =true;
  } 
  else {
    Serial.println("connection failed");
  }

}

void proc_http(){
  if(mNOW_CONE==true){
    if (client.available()) {
      char c = client.read();
//Serial.print(c);
      mResponse3.concat(c);
    }
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      mNOW_CONE=false;
  Serial.print("==== response ==== \n");
  Serial.println("mResponse3=" + mResponse3);
  Serial.print("==== response.END ==== \n");
    } //client.connected
  } //mNOW_CONE
}

void setup() {
  pinMode(SENSOR   ,INPUT);
  pinMode(SENSOR_1 ,INPUT);  
  pinMode(SENSOR_2 ,INPUT);
  pinMode(SENSOR_3 ,INPUT);    
  
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  MsTimer2::set( mSecHttp, timer_func);
  MsTimer2::start();
}

void loop()
{
  proc_http();
}










