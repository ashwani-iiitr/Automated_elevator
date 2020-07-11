#define lift_delay 3000 // time taken to cross a floor

bool la[]= {0, 0, 0, 0};
bool lcup[] = {0, 0, 0, 0};
bool lcdown[] = {0, 0, 0, 0};
   int current_floor =0;
   int i=0,j=0,x=0,y=0;
   int interrupt_pin_in = 2;
   int interrupt_pin_out = 3;
   unsigned long int prev_time = 0;
   const char common = 'a'; 
   const int dataPin = 14;  // blue wire to 74HC595 pin 14
   const int latchPin = 15; // green to 74HC595 pin 12
   const int clockPin = 16; // yellow to 74HC595 pin 11


void open_door(){}
void close_door(){}
void motor_up(){Serial.println("Motor up called");}                    //Servo can be attached
void motor_down(){Serial.println("Motor down called");}
void motor_stop(){Serial.println("Motor is stopped");}

void door_operate(){
  Serial.println("door is operating");
      open_door();
      prev_time = millis();
      while(millis()-prev_time != 5000){}

      close_door();
      Serial.println("Door opened and closed successfully");
  }


void ISR_button_in(){
  Serial.println("ISR triggered in");
  x = PIND;                            //read port D            [3, 2, 1, 0, INTR2, INTR1, RX, TX]
  for(int i=0; i<4; i++)
  la[i] = la[i] || bitRead(x,i+4);   //updated lift_array
  
  }
void ISR_button_out(){
  y = PINB;
  Serial.println("ISR triggered out");
  for(int j=0; j<3; j++){
  lcup[j] = lcup[j] || bitRead(y,j);  //updated liftcall up array
  lcdown[j+1] = lcdown[j+1] || bitRead(y,j+3); //updated liftcall down array
  }  
  } 

void setup() {
  DDRD = 0x00;                         //sets pin 0-7 as input
  DDRB = 0x00;                          //sets pin 6-13 as input
  DDRC = 0b00011111;
  attachInterrupt(digitalPinToInterrupt(interrupt_pin_in), ISR_button_in, RISING);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin_out), ISR_button_out, RISING);
  //reset_lift();
  Serial.begin(9600);
}
void loop(){
    if ((la[i]==0)&&(lcup[i]==0)&&(lcdown[i]==0))
  delay(50);

  else
  {
    if (current_floor == i)
    {
      door_operate();
      la[i]=0;
      lcup[i]=0;
      lcdown[i]=0;
      Serial.println("reset la bcoz same floor");
      
    }

    while((current_floor - i)<0)
    {
       motor_up();
       prev_time = millis();
       if ((la[current_floor + 1] == 1)||(lcup[current_floor + 1] == 1))
       {
        for(;millis()-prev_time<lift_delay;){}    // try lift_delay-500 here     500 for deacceleration
        current_floor++;
        Serial.print("Current floor is  ");
        Serial.println(current_floor);
         byte bits = myfnNumToBits(current_floor) ;
         myfnUpdateDisplay(bits);
        motor_stop();
        door_operate();
        la[current_floor]= 0; 
        lcup[current_floor]= 0;   
         Serial.println("la and lcup 0");
       }
       else
       {
        for(;millis()-prev_time<lift_delay;){}
        current_floor++;
        Serial.print("Current floor is  ");
        Serial.println(current_floor);
         byte bits = myfnNumToBits(current_floor) ;
         myfnUpdateDisplay(bits);
        }
    }

    while((current_floor - i)>0)
    {
       motor_down();
       prev_time = millis();
       if ((la[current_floor - 1] == 1)||(lcdown[current_floor - 1] == 1))
       {
        for(;millis()-prev_time<lift_delay;){}
        current_floor--;
        Serial.print("Current floor is  ");
        Serial.println(current_floor);
         byte bits = myfnNumToBits(current_floor) ;
         myfnUpdateDisplay(bits);
        motor_stop();
        door_operate();
        la[current_floor]= 0; 
        lcdown[current_floor]= 0;  
         Serial.println("la and lcdown 0");
       }
       else
       {
        for(;millis()-prev_time<lift_delay;){}
        current_floor--;
        Serial.print("Current floor is  ");
        Serial.println(current_floor);
         byte bits = myfnNumToBits(current_floor) ;
         myfnUpdateDisplay(bits);
        }
    }
 
   }
 
   i++;
  if(i>3)
  i=0;
  if(digitalRead(19)==HIGH)
    digitalWrite(17, HIGH);
  else
    digitalWrite(17, LOW);

 
}

void myfnUpdateDisplay(byte eightBits) {
  if (common == 'a') {                  // using a common anonde display?
    eightBits = eightBits ^ B11111111;  // then flip all bits using XOR 
  }
  digitalWrite(latchPin, LOW);  // prepare shift register for data
  shiftOut(dataPin, clockPin, LSBFIRST, eightBits); // send data
  digitalWrite(latchPin, HIGH); // update display
}

byte myfnNumToBits(int someNumber) {
  switch (someNumber) {
    case 0:
      return B11111100;
      break;
    case 1:
      return B01100000;
      break;
    case 2:
      return B11011010;
      break;
    case 3:
      return B11110010;
      break;
    case 4:
      return B01100110;
      break;
    case 5:
      return B10110110;
      break;
    case 6:
      return B10111110;
      break;
    case 7:
      return B11100000;
      break;
    case 8:
      return B11111110;
      break;
    case 9:
      return B11110110;
      break;
    case 10:
      return B11101110; // Hexidecimal A
      break;
    case 11:
      return B00111110; // Hexidecimal B
      break;
    case 12:
      return B10011100; // Hexidecimal C or use for Centigrade
      break;
    case 13:
      return B01111010; // Hexidecimal D
      break;
    case 14:
      return B10011110; // Hexidecimal E
      break;
    case 15:
      return B10001110; // Hexidecimal F or use for Fahrenheit
      break;  
    default:
      return B10010010; // Error condition, displays three vertical bars
      break;   
  }
}
