#include <sbus.h>
#include "signal.h"

bfs::SbusData rcsig;
bfs::SbusRx sbus(&Serial2, false);

void InitSbus() {
  sbus.Begin(); 
  Serial3.begin(9600);
}

void GetSignal() {
  bool failSafe, lostFrame;
  
  if (sbus.Read()) {
    rcsig = sbus.data();
   // PrintSignal();
  }
  //rcsig.ch[2]=2000;
  /*rcsig.ch[2]=800;
  rcsig.ch[1]=1025;
  rcsig.ch[3]=1025;
  rcsig.ch[4]=1025;*/
}

void PrintSignal() {
  for (int i = 0; i < 16; i++) {
    Serial.print("Ch"); Serial.print(i); 
    Serial.print(": "); Serial.println(rcsig.ch[i]);
  }
  Serial.println("-----------");
}

void Write() {
  Serial3.print("[");
  Serial3.print(map(rcsig.ch[5],340,1704,500,2500));
  Serial3.print(",");
  if(rcsig.ch[7] < 1000) {
    Serial3.print(3);
  }
  else{
      if(rcsig.ch[8] < 1000) {
    Serial3.print(1);
    }
    else if (rcsig.ch[8]<1300){
      Serial3.print(0);
    }
    else{
    Serial3.print(2);
  }
  }
  Serial3.print(",");
  if(rcsig.ch[6] < 1000){
    Serial3.print(1500);
  }
  else{
    Serial3.print(500);
  }
  Serial3.print(",");
  Serial3.print(map(rcsig.ch[4],340,1704,500,2500));
  Serial3.println("]");
}