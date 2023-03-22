#include <WiFiNINA.h>


char ssid[] = "z10buggy";
char pass[] = "z10buggy";


int button = 0;
int prev_button = 0;


WiFiServer server(5200);


const int LEYE = 2;
const int REYE = 10;


const int LWF = 7;// LEFT WHEEL FOWARD
const int LWB = 8;
const int RWF = 4;
const int RWB = 5; //RIGHT WHEEL BACK


const int LWS = 6; // LEFT WHEEL SPEED 0-225
const int RWS = 9;


const int TRIG = 11;
const int ECHO = 12;




int D = 0;
bool obstacle = false;
bool permission = true;


unsigned int time = 0;
unsigned int newtime = 0;




void setup() {
  Serial.begin(9600);


  WiFi.begin(ssid, pass);
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address:");
  Serial.println(ip);
  server.begin();


  pinMode( LEYE, INPUT );
  pinMode( REYE, INPUT );


  pinMode(LWF, OUTPUT);
  pinMode(LWB, OUTPUT);
  pinMode(RWF, OUTPUT);
  pinMode(RWB, OUTPUT);


  pinMode(LWS, OUTPUT);
  pinMode(RWS, OUTPUT);


  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);


}


void loop() {


  WiFiClient client = server.available();


  stop();
 


  button = client.read();

  


  while(button == 1){

   
    time = millis();

    //Serial.print(time-newtime);
   // Serial.print(" ");
 


    if( (time - newtime) > 500){
      D = US();
     // Serial.print(D);
     // Serial.print(" Distance measured ");
     //Serial.print(time-newtime);
     //Serial.print(" ");
      newtime = time;
    }
 
   
    if(D > 30)
      permission = true;


      while(D > 15 && button != 0 && permission){


        permission = true;


        if(obstacle &&  D > 15){
          obstacle = false;
          Serial.print(" Obstacle cleared ");
          //Serial.print(D);
          client.write('c');
        }
     
        drive();


        time = millis();


        if( (time - newtime) > 500){
          D = US();
          //Serial.print(D);
          //Serial.print(" Distance measured ");
          newtime = time;
              
          if(D <= 15 && !obstacle){
            obstacle = true;
            Serial.print(" obstacle detected ");
            Serial.print(D);
            client.write('d');
            stop();
          }   
      }

      /*  if(D <= 15 && !obstacle){
          obstacle = true;
          Serial.print(" obstacle detected ");
          Serial.print(D);
          client.write('d');
          stop();
        }*/

      button = client.read();


      if(button != 0 ){
        button = 1;}
      
    }
     permission = false;
  }
}


int US(){


  int distance;
  long duration;
  digitalWrite( TRIG, LOW );
  delayMicroseconds(2);


  digitalWrite( TRIG, HIGH );
  delayMicroseconds( 10 );
  digitalWrite( TRIG, LOW );
  duration = pulseIn( ECHO, HIGH );
  distance = duration/58;


  //if(!stopped && distance <= 15){


   
  //}


  return distance;


}




void drive(){


   if( digitalRead( LEYE ) == HIGH && digitalRead( REYE ) == HIGH){
   // Serial.print("low  ");
    digitalWrite(LWF, HIGH);
    digitalWrite(RWF, HIGH);


    analogWrite(LWS, 153); // 60 percent speed
    analogWrite(RWS, 153);
  //  bool stopped = false;
  }


    if( digitalRead( LEYE ) == LOW && digitalRead( REYE ) == LOW){
   // Serial.print("low  ");
    digitalWrite(LWF, HIGH);
    digitalWrite(RWF, HIGH);


    analogWrite(LWS, 153); // 60 percent speed
    analogWrite(RWS, 153);
  //  bool stopped = false;
  }


  if( digitalRead( LEYE ) == HIGH && digitalRead( REYE ) == LOW ){
  //  Serial.println("low  ");
   
    digitalWrite(RWF, HIGH);
    digitalWrite(LWF, HIGH);


    analogWrite(LWS, 204); // 80 speed
    analogWrite(RWS, 63); // 25 speed


 //   bool stopped = false;
  }


    if( digitalRead( LEYE ) == LOW && digitalRead( REYE ) == HIGH ){


    digitalWrite(RWF, HIGH);
    digitalWrite(LWF, HIGH);


    analogWrite(LWS, 63); // 25 speed
    analogWrite(RWS, 204); // 80 speed
   // bool stopped = false;
    }
}


void stop(){


  digitalWrite(LWF, LOW);
  digitalWrite(RWF, LOW);
  digitalWrite(LWB, LOW);
  digitalWrite(RWB, LOW);


}
