/****************************/
/*       RoboRAVE 2014       /
/*           V1.0            /
/****************************/
//

#include <TFT.h>//screen library
#include <Servo.h>//servo library
#include <Motor.h>//motor library

const int loB=530,loW=450,liB=480,liW=400,mB=510,mW=440,riB=560,riW=460,roB=470,roW=380;//black and white thresholds definitions
//const int turnl=20,turnr=-75;//delivering adjustment speed
const int norm=90,turnspd=90,L_diff=0;//speed definitions
const int PREP_ANG=160,DELIV_ANG=100;//servo angle definitions
const int touch=2,s=3;//pin definitions
int lo,li,m,ri,ro;//sensor values
int Flo,Fli,Fm,Fri,Fro;//sensor flags
int touchval=0,swich=1;//touch value and counter(swich)

Servo servo1;//servo definition

void setup()
{
  // put your setup code here, to run once:
  motor.init();//initialize motor
  tft.init();//initialize screen
  servo1.attach(s);//attach servo
  pinMode(touch,INPUT_PULLUP);//define touch sensor as input
  servo1.write(PREP_ANG);//prepare servo
}

void setmotor(int l,int r){motor.run(l,r);}

void RD()//read sensor values
{
  lo=analogRead(A3);//read left out
  li=analogRead(A0);//read left in
  ro=analogRead(A4);//read right out
  ri=analogRead(A1);//read right in
  m=analogRead(A2);//read middle 
  touchval=!(digitalRead(touch));//read touch value
  if(lo>loB)Flo=1;else if(lo<loW)Flo=0;//set flags
  if(li>liB)Fli=1;else if(li<liW)Fli=0;
  if(ro>roB)Fro=1;else if(ro<roW)Fro=0;
  if(ri>riB)Fri=1;else if(ri<riW)Fri=0;
  if(m>mB)Fm=1;else if(m<mW)Fm=0;
//  tft.printf(0,"%d",l);//debug
//  tft.printf(1,"%d",m);
//  tft.printf(2,"%d",r);
}

void deliv()//deliver
{
  setmotor(-turnspd,-turnspd);
  delay(300);
  motor.stop();
  servo1.write(DELIV_ANG);
  for(int i=0;i<=300;i++)
  {
    setmotor(norm+L_diff,norm);
    delay(10);
    setmotor(-norm-L_diff,-norm);
    delay(10);
  }
//  delay(4500);
  motor.stop();
  servo1.write(PREP_ANG);
  RD();
  while(!Fri){RD();setmotor(norm+L_diff,-norm);}
  setmotor(-norm-L_diff,norm);
  delay(100);
  swich=2;
}


void loop()
{
  RD();
  
  if(touchval)
  {
      deliv();//deliver
  }
  
  if((!Flo)&&(!Fli)&&(!Fri)&&(!Fro))//go straight
  {
    setmotor(norm+L_diff,norm);
  }
  
  else if((Flo || Fli)&&(!Fm)&&(!Fri)&&(!Fro)/*&&swich==1*/)//turn left
  {
    while(!Fm)
    {
      setmotor(-norm,norm);
      if(touchval){deliv();break;}
      RD();
    }
    setmotor(norm,-norm);
    delay(100);
  }

  else if((!Flo)&&(!Fli)&&(!Fm)&&(Fri || Fro)/*&&swich==1*/)//turn right
  {
    while(!Fm)
    {
      setmotor(norm,-norm);
      if(touchval){deliv();break;}
      RD();
    }
    setmotor(-norm,norm);
    delay(100);
  }
/*
    else if(Fl&&(!Fm)&&(!Fr)&&swich==2)//turn left
  {
    while(!Fm)
    {
      setmotor(-norm,norm);
      RD();
    }
  }

  else if((!Fl)&&(!Fm)&&Fr&&swich==2)//turn right
  {
    while(!Fm)
    {
      setmotor(norm,-norm);
      RD();
      if((!Fl)&&Fm&&Fr){break;}
    }
  }

  else if((!Fl)&&Fm&&(!Fr)&&swich==2)
  {
    while(!Fr)
    {
      setmotor(-norm,norm);
      RD();
      if((!Fl)&&Fm&&Fr){break;}
    }
  }
  */
  else if((Flo || Fli)&&(Fro || Fri)&&swich==1)//go first T-crossing(turn left)
  {
    setmotor(norm+L_diff,norm);
    delay(100);
    setmotor(-norm-L_diff,-norm);
    delay(50);
    RD();
    while(!Fm){RD();setmotor(-norm-L_diff,norm);}//turn while middle light sensor is white
  }
  
  else if((!Flo)&&(!Fli)&&((Fm&&Fri)||(Fri&&Fro)||(Fm&&Fro))&&swich==2)
  {
    setmotor(norm+L_diff,norm);
    delay(100);
    setmotor(-norm-L_diff,-norm);
    delay(50);
    RD();
    while(!Fri){RD();setmotor(norm+L_diff,-norm);}
    swich=1;
  }
}
