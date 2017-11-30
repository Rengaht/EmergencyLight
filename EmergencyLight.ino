#define USE_ATTINY

#include <CapacitiveSensor.h>

#ifdef USE_ATTINY

#define PIN_R 6
#define PIN_G 7
#define PIN_B 8

#define PIN_BASE 2
#define PIN_HUE 3
#define PIN_BRIGHT 4
#define PIN_MODE 5
#define PIN_PROX 9

#else

#define PIN_R 9
#define PIN_G 10
#define PIN_B 11

#define PIN_BASE 3
#define PIN_HUE 4
#define PIN_BRIGHT 5
#define PIN_MODE 6
#define PIN_PROX 7

#endif


//#define PIN_THRES 1

#define TOUCH_THRES 200
#define PROX_LOW_THRES 20
#define PROX_HIGH_THRES 500

#define MMODE 3

#define BLINK_VEL 200
#define BREATH_VEL 1000

int PIN_THRES=A1;

CapacitiveSensor _cap_hue=CapacitiveSensor(PIN_BASE,PIN_HUE); 
CapacitiveSensor _cap_bright=CapacitiveSensor(PIN_BASE,PIN_BRIGHT); 
CapacitiveSensor _cap_mode=CapacitiveSensor(PIN_BASE,PIN_MODE); 
CapacitiveSensor _cap_prox=CapacitiveSensor(PIN_BASE,PIN_PROX); 

int _mode=0;
int _vhue;
int _vbright=255;
int _vthres;
int a=0;

void changeMode(){

  _mode=(_mode+1)%MMODE;
}


void hueToColor(float h_,float br_,int &r_,int &g_,int &b_){
  int hi=(int)(floor(h_/60.0))%6;
  
  int c=br_;
  float x=(1-abs(fmod(h_/60.0,2)-1))*c;
  int m=br_-c;
  
  
  switch(hi){
    case 0:
      r_=c; g_=x; b_=0;
      break;
    case 1:
      r_=x; g_=c; b_=0;
      break;
    case 2:
      r_=0; g_=c; b_=x;
      break;
    case 3:
      r_=0; g_=x; b_=c;
      break;
    case 4:
      r_=x; g_=0; b_=c;
      break;
    case 5:
      r_=c; g_=0; b_=x;
      break;      
  }
  r_+=m;
  g_+=m;
  b_+=m;
  
  
}





void setup(){

  pinMode(PIN_R,OUTPUT);
  pinMode(PIN_G,OUTPUT);
  pinMode(PIN_B,OUTPUT);
  pinMode(PIN_THRES,INPUT);

  _cap_hue.set_CS_AutocaL_Millis(0xFFFFFFFF);
  _cap_bright.set_CS_AutocaL_Millis(0xFFFFFFFF);
  _cap_mode.set_CS_AutocaL_Millis(0xFFFFFFFF);
  _cap_prox.set_CS_AutocaL_Millis(0xFFFFFFFF);
#ifndef USE_ATTINY 
  Serial.begin(9600); 
#endif
}

void loop(){
  
//  float newthres_=map(analogRead(PIN_THRES),0,1023,100,2000);
  
//  if(abs(newthres_-_vthres)>10){
//    _vthres=newthres_;
//#ifndef USE_ATTINY 
//    Serial.print("thres_= ");
//    Serial.println(_vthres);
//#endif
//  }

//  _vhue=map(newthres_,0,360,100,2000);
  

  
  long thue_=_cap_hue.capacitiveSensor(30);
  long tbright_=_cap_bright.capacitiveSensor(30);
  long tmode_=_cap_mode.capacitiveSensor(30);
//
  long tprox_=_cap_prox.capacitiveSensor(30);
//
//  
    if(tmode_>TOUCH_THRES){
      changeMode();
#ifndef USE_ATTINY 
    Serial.print("change mode=");
    Serial.println(_mode);
#endif    
    
   }else if(tprox_>=PROX_LOW_THRES && tprox_<=PROX_HIGH_THRES){
     if(thue_>TOUCH_THRES){
        _vhue=constrain(map(tprox_,PROX_LOW_THRES,PROX_HIGH_THRES,0,360),0,360);        
     }else if(tbright_>TOUCH_THRES){             
        _vbright=constrain(map(tprox_,PROX_LOW_THRES,PROX_HIGH_THRES,0,255),0,255);     
     }
   }
//     if(thue_>TOUCH_THRES){
//       
//        _vhue=constrain(map(tprox_,_vthres,PROX_HIGH_THRES,0,360),0,360);        
//
//#ifndef USE_ATTINY 
//        Serial.print("hue= ");
//        Serial.println(_vhue);
//#endif
//
//     }else if(tbright_>TOUCH_THRES){
//             
//        _vbright=constrain(map(tprox_,_vthres,PROX_HIGH_THRES,0,255),0,255);     
//
//#ifndef USE_ATTINY 
//        Serial.print("bright= ");
//        Serial.println(_vbright);
//#endif
//     }
//  }
  
  int r,g,b;
  hueToColor(_vhue,_vbright,r,g,b);
  float t=1;
  switch(_mode){
    case 1:
      if(millis()%BLINK_VEL>BLINK_VEL/2) t=0;
      break;  
    case 2:
      t=sin((float)(millis()%BREATH_VEL)/BREATH_VEL*PI);
      break;  
  }
  analogWrite(PIN_R,(float)r*t);
  analogWrite(PIN_G,(float)g*t);
  analogWrite(PIN_B,(float)b*t);

//  a=(a+2)%255;
//  
//  analogWrite(PIN_R,a);
//  analogWrite(PIN_G,a);
//  analogWrite(PIN_B,a);
//  
  delay(5);
}



