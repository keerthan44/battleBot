#include <Ps3Controller.h>

# define player 1

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14; 

// Setting PWM properties for A
const int freq = 30000;
const int pwmChannelA = 0;
const int resolution = 8;
int dutyCycleA = 255;

// Motor B
int motor2Pin1 = 17; 
int motor2Pin2 = 16; 
int enable2Pin = 4; 

// Setting PWM properties for B
const int pwmChannelB  = 1;
int dutyCycleB = 210;
int battery = 0;
bool r2 = false;
bool l2 = false;

void notify(){
  if( battery != Ps3.data.status.battery ){
    battery = Ps3.data.status.battery;
    Serial.print("The controller battery is ");
    if( battery == ps3_status_battery_charging )      Serial.println("charging");
    else if( battery == ps3_status_battery_full )     Serial.println("FULL");
    else if( battery == ps3_status_battery_high )     Serial.println("HIGH");
    else if( battery == ps3_status_battery_low)       Serial.println("LOW");
    else if( battery == ps3_status_battery_dying )    Serial.println("DYING");
    else if( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
    else Serial.println("UNDEFINED");
  }
}

void onConnect(){
  Serial.print("Setting LEDs to player "); Serial.println(player, DEC);
  Ps3.setPlayer(player);
  Serial.println("Connected.");
}


void setup() {
  // Motor A: 
  // sets the pins as outputs 
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  
  // configure LED PWM functionalitites 
  ledcSetup(pwmChannelA, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannelA);

  // Motor B: 
  // sets the pins as outputs 
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
  
  // configure LED PWM functionalitites 
  ledcSetup(pwmChannelB, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable2Pin, pwmChannelB);

  Serial.begin(115200);

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("01:02:03:04:05:06");

  Serial.println("Ready.");

  
}

void loop() {

    if(!Ps3.isConnected())
      return;

    
    if( Ps3.event.button_down.r2 ){
        Serial.println(Ps3.event.button_down.r2);
        r2 = true;
        Serial.println("Setting ps3 controller R2");
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH); 
        
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH); 
        Serial.println("Duty Cycle A: 255");
        Serial.println("Duty Cycle B: 255");

        ledcWrite(pwmChannelA, 255);   
        ledcWrite(pwmChannelB, 255); 


    } else if (Ps3.event.button_up.r2){
      r2 = false;
      ledcWrite(pwmChannelA, 0);   
      ledcWrite(pwmChannelB, 0);
      
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW); 
        
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW); 
    }
    if(r2){
        dutyCycleA = 255;
        if(abs(Ps3.data.analog.stick.lx) > 10 || abs(Ps3.data.analog.stick.ly) > 10){
          if(Ps3.data.analog.stick.ly > 0 ) {
            dutyCycleB = 255;
            dutyCycleA -= (int) abs(Ps3.data.analog.stick.lx) / 1.5;
          } else {
            dutyCycleB = 255 - (int) abs(Ps3.data.analog.stick.lx) / 1.5;
          }
        Serial.print("Duty Cycle A: ");
        Serial.println(dutyCycleA);
        Serial.print("Duty Cycle B: ");
        Serial.println(dutyCycleB);

        ledcWrite(pwmChannelA, dutyCycleA > 200 ? dutyCycleA: 200);   
        ledcWrite(pwmChannelB, dutyCycleB > 200 ? dutyCycleB: 200); 
        } else {
          ledcWrite(pwmChannelA, 255);   
          ledcWrite(pwmChannelB, 255); 

        }

    }
        if( Ps3.event.button_down.l2 ){
        Serial.println(Ps3.event.button_down.r2);
        l2 = true;
        Serial.println("Setting ps3 controller R2");
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW); 
        
        digitalWrite(motor2Pin1, HIGH);
        digitalWrite(motor2Pin2, LOW); 
        Serial.println("Duty Cycle A: 255");
        Serial.println("Duty Cycle B: 255");

        ledcWrite(pwmChannelA, 255);   
        ledcWrite(pwmChannelB, 255); 


    } else if (Ps3.event.button_up.l2){
      l2 = false;
      ledcWrite(pwmChannelA, 0);   
      ledcWrite(pwmChannelB, 0);
      
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW); 
        
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW); 
    }
  // digitalWrite(motor1Pin1, HIGH);
  // digitalWrite(motor1Pin2, HIGH); 
  // delay(2000);

  // // Move the DC motor forward at maximum speed
  // Serial.println("Moving Forward");
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, HIGH); 
  // delay(2000);

  // // Stop the DC motor
  // Serial.println("Motor stopped");
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, LOW);
  // delay(1000);

  // // Move DC motor backwards at maximum speed
  // Serial.println("Moving Backwards");
  // digitalWrite(motor1Pin1, HIGH);
  // digitalWrite(motor1Pin2, LOW); 
  // delay(2000);

  // // Stop the DC motor
  // Serial.println("Motor stopped");
  // digitalWrite(motor1Pin1, LOW);
  // digitalWrite(motor1Pin2, LOW);
  // delay(1000);

  // // Move DC motor forward with increasing speed
  // digitalWrite(motor1Pin1, HIGH);
  // digitalWrite(motor1Pin2, LOW);
  // ledcWrite(pwmChannelA, dutyCycle);   
}