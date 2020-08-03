
// link internet interessante int to string con telegramma https://forum.arduino.cc/index.php?topic=596935.0
//impostazione router casa nuova piano superiore
const char* ssid = "TP-LINK_FF52";
const char* password = "23395985";
#include "WiFi.h" // ESP32 WiFi include
#include "ESPAsyncWebServer.h"// pre creare server
//non funziona l'impostazione ip
// set ip address
IPAddress local_IP(192, 168, 0, 21);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8); //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

#define inpin 16 // pin 1 come ingresso

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

int ningressi = 12;  //numero totale di ingressi, alla fine sulla scheda ne dovremmo prevedere almeno 12 e 4 uscite
int ingpin[12]={16,12,16,12,16,12,16,12,16,12,16, 12};  //numero dei pin in ingresso
int statetrigger[12]={2,2,2,2,2,2,2,2,2,2,2,2}; //salvo gli stati momentaneamente letti e il trigger, riservo i primi posti per gli stati e i successivi per iltrigger rispettivi
int statorip[12]={1,1,1,1,1,1,1,1,1,1,1,1}; // dove salvo lo stato che mi definisce il riposo
int statoattuale[12]={3,3,3,3,3,3,3,3,3,3,3,3};  //dove salvo lo stato attuale
char statetriggerchar[13]="222222222222";  //salvo gli stati momentaneamente letti e il trigger, riservo i primi posti per gli stati e i successivi per iltrigger rispettivi
char stateattualechar[13]="333333333333";  //dove salvo lo stato attuale che poi stamperò
char statoripchar[13]="222222222222";  // dove salvo lo stato che mi definisce il riposo che poi potrò stampare

String ttext="3"; //definizione variabile per lo stato dell'ingresso

void ConnectToWiFi()  //funzione per connettersi al wifi
{
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to "); Serial.println(ssid);
 
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
 
    if ((++i % 16) == 0)
    {
      Serial.println(F(" still trying to connect"));
    }
  }
 
  Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());

    Serial.println("risetto indirizzo ip");
WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);

    Serial.print(F("Connected. My IP address is: "));
  Serial.println(WiFi.localIP());
}

int setinputpin(int* inputpins, int npins){
  Serial.println("settaggio pin inpin input");
  for(int i=0; i<npins;i++){
  pinMode(inputpins[i], INPUT);    // sets input pin
  digitalWrite(inputpins[i], HIGH);// pull up alto
  Serial.print("fase");
  Serial.print(i);
  Serial.print(" settato input pin");
  Serial.println(inputpins[i]);              //avvisiamo di quello che facciamo sulla porta seriale
  }
    return 1;
}


void resettrigger(int numcaselle, int *vettrigger){ //funzione per resettare vettore trigger
  int i=0; //indice per scorrere il vettore
  for(i=0;i<=numcaselle;i++)
    vettrigger[i]=2;
  }

  
int lettf1(int npin){ //funzione lettura pin fase 1
    int vlett=2;  //così dovrei eventualmente scoprire se qui va storto qualcosa
    if(digitalRead(npin)==LOW){
      Serial.print(npin);
      Serial.println(" stato basso"); 
      vlett=0;}
    if(digitalRead(npin)==HIGH){
      Serial.print(npin);
      Serial.println(" stato alto"); 
      vlett=1;}
    return vlett;
  }

//numero totale pin, array con numero pin, array dove salvo stati pin, array in cui salvo il trigger, array con cui faccio il confronto per scegliere se attivare trigger 
int lettf2(int numtotpin, int *numpin, int *statirippin, int *statitriggerf, int *statoattualef){  //funzione lettura ingresso e cambiamento stato trigger
  int i=0;  //indice per scorrere il vettore
  for(i=0;i<=numtotpin;i++){
    statoattualef[i]=lettf1(numpin[i]); //leggo lo stato dei pin
    if(statirippin[i]!=statoattualef[i])
      statitriggerf[i]=1;
    }
    return 1; // avvenuta la corretta lettura
  }

int arrayinttochar(int* intarray, char* chararray, int numcar){ //converte array di nt in array di char poi convertiremo il vrttore char in string 
  for(int i=0; i<numcar; i++){
    if(intarray[i]==0)
      chararray[i]='0';
    if(intarray[i]==1)
      chararray[i]='1';
    if(intarray[i]!=0 && intarray[i]!=1)
      chararray[i]='2';
    }
    chararray[numcar]='\0'; //per chiudere l'array di char, quindi l'array di char dovrà avere una casella in più
    return 1;  //andato a buon fine
  }

int arraychartostring (char *chararray, String stringa){  // NON USARE converte array di char in stringa giusto per ricordarsi come sifa, ma IMPORTANTE l'array di char deve avere '/0'
  stringa = String(chararray);
}
  

void setup() 
{
  Serial.begin(115200);   //inizializzo porta seriale
 
  ConnectToWiFi();        //mi connetto al wifi
  /*
  // Create AsyncWebServer object on port 80
  Serial.println("creazione server su porta 80");
  AsyncWebServer server(80);
  Serial.println("creato server su porta 80");*/
  Serial.println("settaggio pin inpin input");
  pinMode(inpin, INPUT);    // sets input pin
  digitalWrite(inpin, HIGH);// pull up alto
  Serial.println("settato inpin input");
  setinputpin(ingpin, ningressi);   //setto tutti gli ingressi come tali

    //ricezione richiesta info stato sensori
   server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request){
   if(lettf2(ningressi, ingpin, statorip, statetrigger, statoattuale))
      Serial.println("ricevuta richiesta STATE e ho appena letto i pin adesso converto");
    else
     Serial.println("ricevuta richiesta STATE e ho appena letto MALE i pin adesso converto");
    arrayinttochar(statoattuale, stateattualechar, ningressi);
    Serial.println(String(stateattualechar));
    ttext=String(stateattualechar);
    request->send_P(200, "text/plain", ttext.c_str());
  });
  /*  //ricezione richiesta info stato sensori copia DI BACKUP
     server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request){
    ttext=String(lettf1(inpin));
    request->send_P(200, "text/plain", ttext.c_str());
  });*/

    //ricezione richiesta inizializzazione stato sensori
   server.on("/init", HTTP_GET, [](AsyncWebServerRequest *request){
    ttext=String(lettf1(inpin));
    request->send_P(200, "text/plain", ttext.c_str());  //invia stringa
  });
  
    //reset triigger
   server.on("/resettrigger", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("ricevuta richiesta RESETSTATETRIGGER eseguo");
    resettrigger(ningressi, statetrigger);  //resetto array int
    arrayinttochar(statetrigger,statetriggerchar,ningressi);  //lo converto in array char
    ttext=String(statetriggerchar);  //per poi  stampare
    request->send_P(200, "text/plain", ttext.c_str());
  });

  server.begin(); //inizializzazione server
}

    
void loop(){
  
  }
