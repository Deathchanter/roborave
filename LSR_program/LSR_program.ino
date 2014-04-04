#include <TFT.h>
int vallo,valli,valm,valri,valro;
void setup() {
  // put your setup code here, to run once:
  tft.init();
}

void loop() {
  // put your main code here, to run repeatedly: 
  vallo=analogRead(A3);
  valli=analogRead(A0);
  valm=analogRead(A2);
  valri=analogRead(A1);
  valro==analogRead(A4);
  tft.printf(1,"vallo=%d",vallo);
  tft.printf(2,"valli=%d",valli);
  tft.printf(3,"valm=%d",valm);
  tft.printf(4,"valri=%d",valri);
  tft.printf(5,"valro=%d",valro);
}
