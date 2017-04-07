# include "tests.h"
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int LED_RGB_1 = 13;
int LED_RGB_2 = 10;
int LED_RGB_3 = 7;
int LED_RGB_4 = 4;
int LED_RGB_5 = 1;

byte BLACK  = 0;
byte RED    = 1;
byte GREEN  = 2;
byte YELLOW = 3;
byte BLUE   = 4;
byte VIOLET = 5;
byte CYAN   = 6;
byte WHITE  = 7;


String color_name[8];
byte palette[8];
int rShifters = 2;
byte bank_01;
byte bank_02;
byte arrayBank_01[5];
byte arrayBank_02[5];


void setup() {
  Serial.begin(115200);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  palette[BLACK]= 0; // Black
  palette[RED]= 4; // Red
  palette[GREEN]= 2; // Green
  palette[YELLOW]= 6; // Yellow
  palette[BLUE]= 1; // Blue
  palette[VIOLET]= 5; // Violet
  palette[CYAN]= 3; // Cyan
  palette[WHITE]= 0xFF; // White

  
  // Debug..
  color_name[0] = "BLACK";
  color_name[1] = "RED";
  color_name[2] = "GREEN";
  color_name[3] = "YELLOW";
  color_name[4] = "BLUE";
  color_name[5] = "VIOLET";
  color_name[6] = "CYAN";
  color_name[7] = "WHITE";
  
}

unsigned int setRGBled(unsigned int pkg, int led, byte color){
  for (int i = 0; i < 3; i++){
    bitWrite(pkg, i + led, bitRead(color, i));
  }
  return pkg;
}

void write_buffer (unsigned int _package){
  digitalWrite(latchPin, 0);
  Serial.print("buffer_content: 0x");
  for (int i=0; i < 16; i++){
    
    digitalWrite(dataPin, bitRead(_package, i));
    if (bitRead(_package,i) == 1) 
      {Serial.print("1");}
    else
      {Serial.print("0");}
    digitalWrite(clockPin, 0);
    digitalWrite(clockPin, 1);    
  }
  Serial.println("");
  digitalWrite(latchPin, 1);  
} 


void test_1(){
  Serial.println("============= TEST 1 =============");
  Serial.println("Test 1. Todos los LEDs a HIGH Secuencial.");
  unsigned int pkg = 0x0000;
  for (int i=17; i > 0; i--){
    write_buffer(bitWrite(pkg, i , 1));
    delay(200);
  }
  Serial.println("");
}

void test_2_all_red(){
    Serial.println("============= TEST 2 =============");
    Serial.println("Test 2. Todos los LEDs a RED.");
    write_buffer(0x9248); // write: 1001 0010 0100 1000/b
    delay(200);
    Serial.println("");
}

void test_3_all_gren(){
    Serial.println("============= TEST 3 =============");
    Serial.println("Test 3. Todos los LEDs a GREEN.");
    write_buffer(0x4924); // write: 1001 0010 0100 1000/b
    delay(200);
    Serial.println("");
}

void test_4_all_blue(){
    Serial.println("============= TEST 4 =============");
    Serial.println(">> Todos los LEDs a Blue.");
    write_buffer(0x2492); // write: 1001 0010 0100 1000/b
    delay(200);
    Serial.println("");
}

void test_5_all_colors_all_leds_aditive(){
  Serial.println("============= TEST 5 =============");
  Serial.println(">> SET Palette Colors.");
  unsigned int package = 0x0000;
  for (int i=0; i < sizeof(palette); i++){
    write_buffer(0x0000);
    package = setRGBled(package, LED_RGB_5, palette[i]);
    package = setRGBled(package, LED_RGB_4, palette[i]);
    package = setRGBled(package, LED_RGB_3, palette[i]);
    package = setRGBled(package, LED_RGB_2, palette[i]);
    package = setRGBled(package, LED_RGB_1, palette[i]);
    write_buffer(package);
    delay(200);
  }
  Serial.println("");
}

void test_6_all_colors_all_leds_single(){
  Serial.println("============= TEST 5 =============");
  Serial.println(">> SET Palette Colors.");
  unsigned int package = 0;
  for (int i=0; i < sizeof(palette); i++){
    write_buffer(palette[BLACK]);
    package = setRGBled(package, LED_RGB_1, palette[i]);
    write_buffer(package);
    delay(200);
  }
    package = 0;
    for (int i=0; i < sizeof(palette); i++){
    write_buffer(palette[BLACK]);
    package = setRGBled(package, LED_RGB_2, palette[i]);
    write_buffer(package);
    delay(200);
  }
    package = 0;
    for (int i=0; i < sizeof(palette); i++){
    write_buffer(palette[BLACK]);
    package = setRGBled(package, LED_RGB_3, palette[i]);
    write_buffer(package);
    delay(200);
  }
    package = 0;
    for (int i=0; i < sizeof(palette); i++){
    write_buffer(palette[BLACK]);
    package = setRGBled(package, LED_RGB_4, palette[i]);
    write_buffer(package);
    delay(200);
  }
    package = 0;
    for (int i=0; i < sizeof(palette); i++){
    write_buffer(palette[BLACK]);
    package = setRGBled(package, LED_RGB_5, palette[i]);
    write_buffer(package);
    delay(200);
  }
  Serial.println("");
}
void test_7_ramp(){
  
}

void just_blink(int blinks){
  for (int i=0; i < blinks; i++){
    write_buffer(0xFFFE);
    delay(100);
    write_buffer(0x0000);
    delay(250);
    }
}


void check_leds_integrity(){
  just_blink(3); // Start sec
  test_1();
  test_2_all_red();
  test_3_all_gren();
  test_4_all_blue();
  test_5_all_colors_all_leds_aditive();
  test_6_all_colors_all_leds_single();
  just_blink(2); // End sec
  delay(1000);
}

void loop() {
    check_leds_integrity();
}
