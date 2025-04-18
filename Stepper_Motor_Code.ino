#include <Stepper.h> 

double stepsPerRevolution = 2048;
#define TOGGLE_PIN 3
#define ccwPin 4
int switchCW=3;
int switchCCW=4;
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);  // Pin inversion to make the library work

void setup() { 
  pinMode(switchCW, INPUT_PULLUP);
  pinMode(switchCCW, INPUT_PULLUP);
  myStepper.setSpeed(10);  
  Serial.begin(9600); 

  } 
  
  void loop() {  
    bool ccw = digitalRead(ccwPin);
   // int toggleState=digitalRead(TOGGLE_PIN);
   if(digitalRead(switchCCW)==LOW)
   {
   dirStatus=1;

   }else if(digitalRead(switchCW==LOW))
   {
    dirStatus=2;

   }else
   {
   dirstatus=3;
   }
   
   }
    if (ccw==LOW) {
      Serial.println("Toggle is low");
    } else {
      Serial.println(ccw);
    }
    // 1 rotation counterclockwise:  
    //Serial.println("counterclockwise");    
    myStepper.step(stepsPerRevolution);  
    //delay(1000); 
     
   // 1 rotation clockwise: 
   //Serial.println("clockwise");  
   myStepper.step(-stepsPerRevolution); 
   //delay(1000); 
}
