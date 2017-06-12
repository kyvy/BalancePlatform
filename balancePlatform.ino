#include <Servo.h>
#include <string.h>

#define BAUD 256000

#define INPUT_SIZE 64
#define START_MARKER '<'
#define END_MARKER '>'

#define PITCH_SERVO_PIN 7
#define ROLL_SERVO_PIN 8

char receivedChars[INPUT_SIZE];
char parseChars[INPUT_SIZE];
boolean newData = false;

Servo pitch;  // rotate about x axis
Servo roll;   // rotate about y axis
int x, y;     // servo positions

void recv(){
  static boolean receiving = false;
  static byte ndx = 0;
  char rc;

  if ((Serial.available() > 0) && !newData) {
    rc = Serial.read();
    
    if (receiving) {
      if (rc != END_MARKER) {
        receivedChars[ndx] = rc;
        ndx++;
        
        if (ndx >= INPUT_SIZE)
          ndx = INPUT_SIZE - 1;
     
      } else {
        receivedChars[ndx] = '\0'; // terminate the string
        receiving = false;
        ndx = 0;
        newData = true;
        
//        Serial.println(receivedChars);
        strcpy(parseChars, receivedChars);  // the data is modified during parsing
      }
      
    } else if (rc == START_MARKER) {
      receiving = true;
    }
  }
}

void parseReceived() {
  char* index;

  index = strtok(parseChars, ",");  // get the first part of the string
  x = atoi(index);

  index = strtok(NULL, ",");        // continues where the previous call left off
  y = atoi(index); 
//  Serial.println(x);
//  Serial.println(y);
}

void setup() {
  Serial.begin(BAUD);
  pitch.attach(PITCH_SERVO_PIN);
  roll.attach(ROLL_SERVO_PIN);
}

void loop() {
  recv();
  
  if (newData) {
    parseReceived();
    pitch.writeMicroseconds(x);
    roll.writeMicroseconds(y);
    newData = false;
  }
}


