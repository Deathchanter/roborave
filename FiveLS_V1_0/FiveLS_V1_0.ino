/****************************/
/*       RoboRAVE 2014       /
/*           V1.0            /
/****************************/
//

#include <TFT.h>//screen library
#include <Servo.h>//servo library
#include <Motor.h>//motor library

const int Bfa=480,Wfa=350;//black and white thresholds definitions
//const int turnl=20,turnr=-75;//delivering adjustment speed
const int norm=50,turnspd=50,L_diff=4;//speed definitions
const int PREP_ANG=180,DELIV_ANG=100;//servo angle definitions
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

void setmotor(int l,int r){motor.run(-r,-l);}

void RD()//read sensor values
{
  lo=analogRead();//read left out
  li=analogRead(A0);//read left in
  ro=analogRead();//read right out
  ri=analogRead(A1);//read right in
  m=analogRead(A2);//read middle 
  touchval=!(digitalRead(touch));//read touch value
  if(lo>Bfa)Flo=1;else if(lo<Wfa)Flo=0;//set flags
  if(li>Bfa)Fli=1;else if(li<Wfa)Fli=0;
  if(ro>Bfa)Fro=1;else if(ro<Wfa)Fro=0;
  if(ri>Bfa)Fri=1;else if(ri<Wfa)Fri=0;
  if(m>Bfa)Fm=1;else if(m<Wfa)Fm=0;
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
  for(int i=0;i<=300)
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
      setmotor(-norm/2-10,norm/2+10);
      if(touchval){deliv();break;}
      RD();
    }
    setmotor(norm/2+10,-norm/2-10);
    delay(100);
  }

  else if((!Flo)&&(!Fli)&&(!Fm)&&(Fri || Fro)/*&&swich==1*/)//turn right
  {
    while(!Fm)
    {
      setmotor(norm/2+10,-norm/2-10);
      if(touchval){deliv();break;}
      RD();
    }
    setmotor(-norm/2-10,norm/2+10);
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
