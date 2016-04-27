#include <dht11.h>
#include "IRremote.h"
#include <VirtualWire.h>

IRsend irsend;

dht11 DHT;
#define DHT11_PIN 4
char inData[20]; // Allocate some space for the string
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character
String content = "";
int LED = 2;
int khz=38;
int state;
unsigned signal[] ={3450,3450,900,2600,850,750,1000,2550,900,2600,850,850,900,2550,900,850,900,850,850,2600,850,850,900,2600,850,2600,850,850,900,2600,850,850,900,800,900,2600,1000,2450,900,850,850,850,900,850,900,800,900,850,900,850,900,2550,900,2550,900,800,900,850,900,850,900,850,850,850,900,850,3450,3450,1000,2450,900,850,900,2550,900,2550,900,850,900,2550,900,750,950,850,900,2600,850,850,900,2550,900,2550,900,850,900,2550,900,850,900,850};
static uint32_t timer;

void setup(){
  pinMode(LED, OUTPUT);
  vw_setup(2000); // Bits per sec
  Serial.begin(9600);
  state=0;
  timer = -9999999; // start timing out right away
}
 
void loop(){
  DHT.read(DHT11_PIN);    // READ DATA
  int h=DHT.humidity;
  int t=DHT.temperature;
  Serial.print(h);
  Serial.print("-");
  Serial.println(t);
  //send("do nothing");
  delay (1000);
  
  while (Serial.available() > 0) // Don't read unless
                                 // there you know there is data
  {
    if(index < 19) // One less than the size of the array
    {
      inChar = Serial.read(); // Read a character
      inData[index] = inChar; // Store it
      index++; // Increment where to write next
      content.concat(inChar);
      inData[index] = '\0'; // Null terminate the string
    }
  }
  String string="condOn";
  if (string.compareTo(content)==-10){
    //digitalWrite(LED, HIGH);
    irsend.sendRaw(signal, sizeof(signal)/sizeof(int), khz);
    Serial.println(string);
    //delay(1000);
    //digitalWrite(LED, LOW);
  }
  string="condOff";
  if (string.compareTo(content)==-10){
    //digitalWrite(LED, HIGH);
    irsend.sendRaw(signal, sizeof(signal)/sizeof(int), khz);
    Serial.println(string);
    //delay(1000);
    //digitalWrite(LED, LOW);
  }
  string="risOn";
  if (string.compareTo(content)==-10){
    digitalWrite(LED, HIGH);
    Serial.println(string);
    delay(1000);
    digitalWrite(LED, LOW);
  }
  string="risOff";
  if (string.compareTo(content)==-10){
    digitalWrite(LED, HIGH);
    Serial.println(string);
    delay(1000);
    digitalWrite(LED, LOW);
  }
  string="acqOn";
  if (string.compareTo(content)==-10){
    Serial.println("startWater");
    //send("waterOn");
    delay(1000);
    //Serial.println("startWater");
    state=1;
  }
  string="acqOff";
  if (string.compareTo(content)==-10){
    //Serial.println("stopWater");
    Serial.println("startWater");
    //send("waterOff");
    delay(1000);
    state=2;
  }
  content="";
  
  /*
  if(state==0)send("do nothing");
  if(state==1)send("acqOn");
  if(state==2)send("acqOff");*/
  
  switch (state){
    case 0: 
      send("do nothing");
      Serial.println("do nothing");
    break;
    case 1: 
      send("waterOn");
      Serial.println("startWater");
    break;
    case 2:
      send("waterOff");
      Serial.println("stopWater");
    break;
  }
  
}


void send (char *message){
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone
}
