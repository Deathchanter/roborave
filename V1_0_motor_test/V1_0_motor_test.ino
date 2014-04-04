#include <Motor.h>
void setup() {
  // put your setup code here, to run once:
motor.init();
}

void loop() {
  // put your main code here, to run repeatedly: 
  motor.run(50,50);
  delay(1000);
  motor.run(0,0);
  delay(1000);
}
