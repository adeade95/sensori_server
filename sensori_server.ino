
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

int ningressi = 1;  //numero totale di ingressi, alla fine sulla scheda ne dovremmo prevedere almeno 12 e 4 uscite
int pin[1]={16};  //numero dei pin in ingresso
int statetrigger[2]={1,2}; //salvo gli stati momentaneamente letti e il trigger, riservo i primi posti per gli stati e i successivi per iltrigger rispettivi
int statorip[1]={1}; // dove salvo lo stato che mi definisce il riposo
int statoattuale[1];  //dove salvp lo stato attuale

String ttext="3"; //definizione variabile per lo stato dell'ingresso

void ConnectToWiFi()
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

char* cifretolett (int nvett[], int iniz, int fin){  //funzione che ricevuto in ingresso un vettore di interi e le riunisce a gruppi di 4,     da sistemare ancora è da capire come usare, altrimenti copiamo e incolliamo solo il contenuto
  char lettere [fin-iniz];
  for(int i=iniz;i<=fin;i++)
   lettere [i]= (char) nvett[i];
  return lettere;
}

void resettrigger(int numcaselle, int *vettrigger){ //funzione per resettare vettore trigger
  int i=0; //indice per scorrere il vettore
  for(i=0;i<=numcaselle;i++)
    vettrigger[i]=0;
  }

  
int lettf1(int npin){ //funzione lettura pin fase 1
    int vlett=2;
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
  
int lettf2(int numtotpin, int *numpin, int *statipin, int *statitriggerf, int *statoattualef){  //funzione lettura ingresso e cambiamento stato trigger
  int i=0;  //indice per scorrere il vettore
  for(i=0;i<=numtotpin;i++){
    statipin[i]=lettf1(numpin[i]);
    if(statipin[i]!=statoattualef[i])
      statitriggerf[i]=1;
    }
    return 1; // avvenuta la corretta lettura
  }

int arrayinttochar(int* intarray, char* chararray, int numcar){ //converte array di nt in array di char poi convertiremo il vrttore char in string 
  for(int i=0; i<numcar; i++){
    if(intarray[i]=0)
      chararray[i]='0';
    if(intarray[i]=1)
      chararray[i]='1';
    if(intarray[i]!=0 && intarray[i]!=1)
      chararray[i]='2';
    }
    chararray[numcar]='\0'; //per chiudere l'array di char, quindi l'array di char dovrà avere una casella in più
  }
  
int creastringa(int* vn, int n){//funzione creazione vettore di caratteri da inviare 
  for(int i=0;i<n;i++){
    
    }
    return 1;
  }

void setup() 
{
  Serial.begin(115200);
 
  ConnectToWiFi();
  /*
  // Create AsyncWebServer object on port 80
  Serial.println("creazione server su porta 80");
  AsyncWebServer server(80);
  Serial.println("creato server su porta 80");*/
  Serial.println("settaggio pin inpin input");
  pinMode(inpin, INPUT);    // sets the digital pin 4 led pin as output
  digitalWrite(inpin, HIGH);// pull up alto
  Serial.println("settato inpin input");

    //ricezione richiesta info stato sensori
   server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request){
    ttext=String(lettf1(inpin));
    request->send_P(200, "text/plain", ttext.c_str());
  });

    //ricezione richiesta inizializzazione stato sensori
   server.on("/init", HTTP_GET, [](AsyncWebServerRequest *request){
    ttext=String(lettf1(inpin));
    request->send_P(200, "text/plain", ttext.c_str());  //invia stringa
  });

  server.begin(); //inizializzazione server
}

    //ricezione richiesta reset trigger da creare-----------------------------------
    
void loop(){}
