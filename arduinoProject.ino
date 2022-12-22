#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <IRremote.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 10       // LCD Pin
#define RST_PIN 9       // LCD Pin
#define LED_G 6         // Green LED pin
#define LED_R 5         // Red LED pin
#define lock 3          // Servo
const int RECV_PIN = 8; // IR remote
const int buzzer = 2;   // buzzer pin
const int Btn = 7;      // button pin

const int RFID_NUMBER = 3;        // number of RFID cards
bool entered = false;             // boolean to check whether a person is leaving
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create c instance
Servo myServo;                    // create servo object to control a servo
IRrecv IR(RECV_PIN);              // create instance of 'irrecv'
#define button1 3125149440        // remote buttons
#define button2 3108437760        // remote button 2
#define button3 3091726080        // remote button 3
#define button4 3141861120        // remote button 4
#define button5 3208707840        // remote button 5
#define button6 3158572800        // remote button 6
#define button7 4161273600        // remote button 7
#define button8 3927310080        // remote button 8
#define button9 4127850240        // remote button 9
#define button0 3860463360        // remote button 0
#define okk 3810328320            // remote button ok
#define up 3877175040             // remote button up
#define down 2907897600           // remote button down
#define left 4144561920           // remote button left
#define right 2774204160          // remote button right
#define star 3910598400           // remote button *
#define hash 4061003520           // remote button #
unsigned long password[4] = {button1, button2, button3, button4};
int count; // check which person just entered/left
int totalTime[RFID_NUMBER] = {0, 0, 0};
bool enter[RFID_NUMBER] = {false, false, false};
String rfids[RFID_NUMBER] = {"B9 41 9D 84", "17 93 34 33", "59 3E E7 59"}; // RFID cards
double ttime[RFID_NUMBER] = {0.0, 0.0, 0.0};                               // time of each card
bool isDoctor[RFID_NUMBER] = {false, false, true};
int timesEntered[RFID_NUMBER] = {0, 0, 0};
bool presidentEntered = false;
String names[RFID_NUMBER] = {"Antonio", "Joseph", "Dr. Fadi"}; // name of each card
  int Si2 = 1975;
  int LaS2 = 1864;
  int La2 = 1760;
  int SolS2 = 1661;
  int Sol2 = 1567;
  int FaS2 = 1479;
  int Fa2 = 1396;
  int Mi2 = 1318;
  int ReS2 = 1244;
  int Re2 = 1174;
  int DoS2 = 1108;
  int Do2 = 1046;

// Low Octave
int Si = 987;
int LaS = 932;
int La = 880;
int SolS = 830;
int Sol = 783;
int FaS = 739;
int Fa = 698;
int Mi = 659;
int ReS = 622;
int Re = 587;
int DoS = 554;
int Do = 523;

// define the notes
int rounda = 0;
int roundp = 0;
int white = 0;
int whitep = 0;
int black = 0;
int blackp = 0;
int quaver = 0;
int quaverp = 0;
int semiquaver = 0;
int semiquaverp = 0;
int bpm = 120;
void setup()
{
    IR.enableIRIn(); // Start the receiver
    IR.disableIRIn();
    Serial.begin(9600);
    SPI.begin();
    pinMode(buzzer, OUTPUT); // set the buzzer pin mode
    myServo.attach(lock);    // attaches the servo on pin 3 to the servo object
    mfrc522.PCD_Init();      // Initialize MFRC522
    pinMode(LED_G, OUTPUT);  // set the LED pin mode
    pinMode(LED_R, OUTPUT);  // set the LED pin mode
    pinMode(Btn, INPUT);     // set the button pin mode
    pinMode(lock, OUTPUT);   // set the servo pin mode
    lcd.init();              // initialize the lcd
    lcd.backlight();         // turn on the backlight
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Scan Your RFID ");
    lcd.setCursor(0, 1);
    lcd.print("   Door Locked   ");
    myServo.write(0); // close the door
    delay(500);
    IR.enableIRIn();
    black = 35000 / bpm;
    blackp = black * 1.5;
    white = black * 2;
    whitep = white * 1.5;
    rounda = black * 4;
    roundp = rounda * 1.5;
    quaver = black / 2;
    quaverp = quaver * 1.5;
    semiquaver = black / 4;
    semiquaverp = semiquaver * 1.5;
}
void loop()
{
    if (digitalRead(Btn) == HIGH)
    { // if button is pressed
        IR.disableIRIn();
        digitalWrite(LED_G, HIGH); // turn on green led
        lcd.setCursor(0, 1);       // go to second line
        myServo.write(90);         // open the door
        tone(buzzer, 2000);        // make a sound
        lcd.setCursor(0, 1);       // go to second line
        lcd.print("  Door UnLocked   ");
        delay(1000);              // wait for 1 second
        noTone(buzzer);           // stop the sound
        entered = true;           // set entered to true
        digitalWrite(LED_G, LOW); // turn off green led
        IR.enableIRIn();
    }
    if (IR.decode() && IR.decodedIRData.decodedRawData == up)
    {
        lcd.clear();
        delay(500);
        for (int i = 0; i < RFID_NUMBER; i++)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(names[i] + " entered");
            lcd.setCursor(0, 1);
            lcd.print("a total of: ");
            lcd.print(timesEntered[i]);
            delay(1500);
        }
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("   Door Locked   ");
        lcd.setCursor(0, 0);
        lcd.print(" Scan Your RFID ");
        IR.resume();
    }
    else if (IR.decode() && IR.decodedIRData.decodedRawData == down)
    {
        IR.resume();
        changePasssword();
        IR.resume();
    }
    else if (IR.decode() && IR.decodedIRData.decodedRawData == star)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("reset Successful!");
        for (int i = 0; i < RFID_NUMBER; i++)
        {
            totalTime[i] = 0;
        }
        IR.resume();
        delay(1500);
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("   Door Locked   ");
        lcd.setCursor(0, 0);
        lcd.print(" Scan Your RFID ");
    }
    else if (IR.decode() && IR.decodedIRData.decodedRawData == hash)
    { // if IR signal is received and it is equal to the code of the IR remote which is "#"
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Total time for");
        lcd.setCursor(0, 1);
        lcd.print("students is: ");
        delay(1000);
        for (int i = 0; i < RFID_NUMBER; i++)
        { // loop through all the cards
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(names[i]); // print the name of the card
            lcd.setCursor(0, 1);
            int timme = totalTime[i]; // get the total time for each person
            int hour = timme / 3600;  // convert the time to hours, minutes and seconds
            lcd.print("h:");
            lcd.print(hour);
            int minute = (timme % 3600) / 60;  // convering seconds to minutes
            int seconds = (timme % 3600) % 60; // converting seconds to remaining seconds
            lcd.print(" m:");
            lcd.print(minute);
            lcd.print(" s:");
            lcd.print(seconds);
            delay(2000);
        }
        IR.resume();
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("   Door Locked   ");
        lcd.setCursor(0, 0);
        lcd.print(" Scan Your RFID ");
    }
    else if (IR.decode() && entered && IR.decodedIRData.decodedRawData == okk)
    { // if the button "OK" is pressed and the door is opened
        IR.disableIRIn();
        lcd.clear();
        digitalWrite(LED_G, HIGH);
        myServo.write(0);
        lcd.print("Bye President!");
        presidentEntered = false;
        sound();
        delay(2000);
        lcd.clear();
        digitalWrite(LED_G, LOW);
        lcd.setCursor(0, 1);
        lcd.print("   Door Locked   ");
        lcd.setCursor(0, 0);
        lcd.print(" Scan Your RFID ");
        entered = false;
        IR.enableIRIn();
    }
    else if (IR.decode() && !entered && check()) // in order to get the value for each button in the IR remote we need to press the button and then go to the serial monitor and copy the value by doing Serial.println(IR.decodedIRData.decodedRawData);
    {
        int c = 0;
        bool che = true;                                       // che checks if the codes are correct and c counts the number of codes the user entered                                                         // if IR remote is pressed
        double start = millis();                               // start time
        lcd.clear();                                           // clear the lcd
        che &= IR.decodedIRData.decodedRawData == password[0]; // check if the first code is correct
        IR.resume();                                           // resume the IR
        lcd.print("*");                                        // print * to show that the a code is entered
        while (c < 4)
        {                                // while time is less than 8 seconds and the user entered less than 4 codes
            if (millis() - start > 8000) // if time is more than 8 seconds
            {
                che = false; // set it to false in order to prevent the door from opening if any of the digits pressed are right
                c = 0;
                break;
            }
            if (IR.decode())
            {
                if (c == 1)
                {
                    che &= IR.decodedIRData.decodedRawData == password[1];
                    lcd.print("*"); // check if the second code is correct
                }
                else if (c == 2)
                {
                    che &= IR.decodedIRData.decodedRawData == password[2];
                    lcd.print("*"); // check if the third code is correct
                }
                else if (c == 3)
                {
                    che &= IR.decodedIRData.decodedRawData == password[3];
                    lcd.print("*"); // check if the fourth code is correct
                }
                delay(200); // wait for 0.3 seconds between each press
                c++;        // increment c
                IR.resume();
            }
        }
        IR.disableIRIn();
        if (che)
        { // if all codes are correct
            lcd.clear();
            lcd.print("Wlcm President!");
            digitalWrite(LED_G, HIGH); // turn on green led
            lcd.setCursor(0, 1);
            myServo.write(90);
            tone(buzzer, 2000); // make a sound
            delay(1000);        // wait for 1 second
            noTone(buzzer);     // stop the sound
            delay(1000);
            digitalWrite(LED_G, LOW); // turn off green led
            myServo.write(0);         // close the door
            delay(100);
            lcd.clear();
            presidentEntered = true;
            sound1();
        }
        lcd.clear();
        c = 0;
        che = false;
        lcd.setCursor(0, 0);           // go to first line
        lcd.print(" Scan Your RFID "); // print message
        lcd.setCursor(0, 1);
        lcd.print("   Door Locked   ");
        delay(1000);
        IR.enableIRIn();
    }
    else
    {
        IR.resume();
        // Look for new cards
        if (!mfrc522.PICC_IsNewCardPresent())
        {
            return;
        }
        // Select one of the cards
        if (!mfrc522.PICC_ReadCardSerial())
        {
            return;
        }
        // Show UID on serial monitor
        String content = "";
        byte letter;
        for (byte i = 0; i < mfrc522.uid.size; i++)
        {
            content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ")); // add space before each byte
            content.concat(String(mfrc522.uid.uidByte[i], HEX));                // convert each byte to HEX
        }
        content.toUpperCase(); // convert to uppercase
        bool ok = false;       // set ok to false
        for (int i = 0; i < RFID_NUMBER; i++)
        {
            if (rfids[i] == content.substring(1))
            { // if the card is found
                ok = true;
                count = i;
                break;
            }
        }
        if (!entered && ok) // if the card is found and the door is not opened
        {
            if (isDoctor[count] || presidentEntered || enter[2])
            {
                IR.disableIRIn();
                digitalWrite(LED_G, HIGH);
                lcd.setCursor(0, 1);
                enter[count] = true;     // set enter to true i.e the person entered
                ttime[count] = millis(); // set the time i.e the time the person entered
                lcd.print(" Door Un-Locked ");
                myServo.write(90);
                tone(buzzer, 2000);
                delay(1000);
                noTone(buzzer);
                delay(1000);
                digitalWrite(LED_G, LOW); // turn off green led
                myServo.write(0);
                delay(100);
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Welcome " + names[count]);
                delay(2000);
                lcd.setCursor(0, 1);
                lcd.print("   Door Locked   ");
                lcd.setCursor(0, 0);
                lcd.print(" Scan Your RFID ");
                delay(100);
                IR.enableIRIn();
            }
            else
            {
                IR.disableIRIn();
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("You are not");
                lcd.setCursor(0, 1);
                lcd.print("Allowed to enter");
                digitalWrite(LED_R, HIGH);
                delay(2000);
                digitalWrite(LED_R, LOW);
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print("   Door Locked   ");
                lcd.setCursor(0, 0);
                lcd.print(" Scan Your RFID ");
                delay(100);
                IR.enableIRIn();
            }
        }
        else if (ok && entered) // if the card is found and the door is opened
        {                       // this will execute when the person leaves, i.e when the button is pressed
            IR.disableIRIn();
            digitalWrite(LED_G, HIGH);
            myServo.write(0);
            entered = false;
            ttime[count] = millis() - ttime[count]; // time the person stayed
            lcd.clear();
            totalTime[count] += ttime[count] / 1000; // clear the lcd
            lcd.setCursor(0, 0);
            lcd.print("Goodbye " + names[count]); // print the name of the person who left
            lcd.setCursor(0, 1);
            if (enter[count])
            { // if the person entered using the card
                int timme = ttime[count] / 1000;
                int hour = timme / 3600; // convert the time to hours, minutes and seconds
                lcd.print("h:");
                lcd.print(hour);
                int minute = (timme % 3600) / 60;  // convering seconds to minutes
                int seconds = (timme % 3600) % 60; // converting seconds to remaining seconds
                lcd.print(" m:");
                lcd.print(minute);
                lcd.print(" s:");
                lcd.print(seconds);
            }
            enter[count] = false; // set enter to false i.e the person left
            timesEntered[count]++;
            delay(2000);
            lcd.setCursor(0, 1);
            lcd.print("   Door Locked   ");
            lcd.setCursor(0, 0);
            lcd.print(" Scan Your RFID ");
            digitalWrite(LED_G, LOW);
            delay(100);
            IR.enableIRIn();
        }
        else
        {
            IR.disableIRIn();
            lcd.setCursor(0, 1);            // go to second line
            lcd.print("Invalid RFID Tag");  // print message
            digitalWrite(LED_R, HIGH);      // turn on red led
            delay(1000);                    // wait for 0.5 second
            digitalWrite(LED_R, LOW);       // turn off red led
            lcd.setCursor(0, 1);            // go to second line
            lcd.print("   Door Locked   "); // print message
            //  lcd.print(content.substring(1));  //this will print the UID of the card if we need it in the future
            delay(1000);
            IR.enableIRIn();
        }
    }
}

void sound()
{
    tone(buzzer, 261);
    // Waits some time to turn off
    delay(200);
    // Turns the buzzer off
    noTone(buzzer);
    // Sounds the buzzer at the frequency relative to the note D in Hz
    tone(buzzer, 293);
    delay(200);
    noTone(buzzer);
    // Sounds the buzzer at the frequency relative to the note E in Hz
    tone(buzzer, 329);
    delay(200);
    noTone(buzzer);
    // Sounds the buzzer at the frequency relative to the note F in Hz
    tone(buzzer, 349);
    delay(200);
    noTone(buzzer);
    // Sounds the buzzer at the frequency relative to the note G in Hz
    tone(buzzer, 392);
    delay(200);
    noTone(buzzer);
}

void sound1()
{
    tone(buzzer, Mi, black);
    delay(black + 50);
    tone(buzzer, La, black);
    delay(black + 50);
    tone(buzzer, Si, black);
    delay(black + 50);
    tone(buzzer, Do2, black);
    delay(black + 50);
    tone(buzzer, La, black);
}

void changePasssword()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Old");
    lcd.setCursor(0, 1);
    lcd.print("Password: ");
    delay(1500);
    lcd.clear();
    if (!verifyPassword()) // if the password is wrong
    {
        goto label;
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter New");
    lcd.setCursor(0, 1);
    lcd.print("Password: ");
    delay(1000);
    lcd.clear();
    IR.resume();
    if (passwordCheck()) // if the two passwords entered are correct
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        tone(buzzer, 2000);
        lcd.print("You Changed");
        lcd.setCursor(0, 1);
        lcd.print(" the Password!");
        digitalWrite(LED_G, HIGH);
        delay(2000);
        noTone(buzzer);
    }
    else
    { // if the two passwords entered do not match or there has been a timeout
        digitalWrite(LED_R, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Passwords do not");
        lcd.setCursor(0, 1);
        lcd.print("Match!");
        delay(1500);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Please try");
        lcd.setCursor(0, 1);
        lcd.print("Again");
        digitalWrite(LED_R, LOW);
        delay(2000);
    }
label:
    digitalWrite(LED_G, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);           // go to first line
    lcd.print(" Scan Your RFID "); // print message
    lcd.setCursor(0, 1);
    lcd.print("   Door Locked   ");
    delay(1000);
    IR.resume();
    IR.enableIRIn();
}

bool verifyPassword()
{ // function to verify the password
    bool che = true;
    double start2 = millis();
    IR.resume();
    while (!IR.decode()) // while the IR is not decoded
    {
        if (millis() - start2 > 8000)
        {
            Serial.print(millis());
            Serial.print(" ");
            Serial.print(start2);
            return false;
        }
    }
    int c = 0;
    double start = millis();                               // start time
    lcd.clear();                                           // clear the lcd
    che &= IR.decodedIRData.decodedRawData == password[0]; // check if the first code is correct
    IR.resume();                                           // resume the IR
    lcd.print("*");                                        // print * to show that the a code is entered
    while (c < 4)
    {                                // while time is less than 10 seconds and the user entered less than 4 codes
        if (millis() - start > 10000) // if time is more than 10 seconds
        {
            che = false; // set it to false in order to prevent the door from opening if any of the digits pressed are right
            c = 0;
            IR.resume();
            break;
        }
        if (IR.decode())
        {
            if (c == 1)
            {
                che &= IR.decodedIRData.decodedRawData == password[1];
                lcd.print("*"); // check if the second code is correct
            }
            else if (c == 2)
            {
                che &= IR.decodedIRData.decodedRawData == password[2];
                lcd.print("*"); // check if the third code is correct
            }
            else if (c == 3)
            {
                che &= IR.decodedIRData.decodedRawData == password[3];
                lcd.print("*"); // check if the fourth code is correct
            }
            delay(200); // wait for 0.3 seconds between each press
            c++;        // increment c
            IR.resume();
        }
    }
    return che;
}

bool passwordCheck()
{
    bool che = true;
    double start2 = millis();
    unsigned long var[4] = {0, 0, 0, 0};
    while (!IR.decode()) // while the IR is not decoded
    {
        if (millis() - start2 > 8000)
        { // if time is more than 8 seconds
            return false;
        }
    }
    lcd.clear();                              // clear the lcd
    var[0] = IR.decodedIRData.decodedRawData; // check if the first code is correct
    IR.resume();                              // resume the IR
    lcd.print("*");
    int c = 0;
    while (c < 4)
    {                                 // while time is less than 8 seconds and the user entered less than 4 codes
        if (millis() - start2 > 8000) // if time is more than 8 seconds
        {
            return false;
        }
        if (IR.decode())
        {
            if (c == 1)
            {
                var[1] = IR.decodedIRData.decodedRawData; // store the second input in var[1]
                lcd.print("*");                           // check if the second code is correct
            }
            else if (c == 2)
            {
                var[2] = IR.decodedIRData.decodedRawData; // store the third input in var[2]
                lcd.print("*");                           // check if the third code is correct
            }
            else if (c == 3)
            {
                var[3] = IR.decodedIRData.decodedRawData; // store the fourth input in var[3]
                lcd.print("*");                           // check if the fourth code is correct
            }
            delay(200); // wait for 0.3 seconds between each press
            c++;        // increment c
            IR.resume();
        }
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please enter");
    lcd.setCursor(0, 1);
    lcd.print("Password Again: ");
    delay(1500);
    lcd.clear();
    start2 = millis();
    c = 0;
    while (!IR.decode())
    {
        if (millis() - start2 > 8000)
        {
            return false;
        }
    }
    che &= var[0] == IR.decodedIRData.decodedRawData; // check if the first codes match
    IR.resume();                                      // resume the IR
    lcd.print("*");
    while (c < 4)
    {                                 // while time is less than 8 seconds and the user entered less than 4 codes
        if (millis() - start2 > 8000) // if time is more than 8 seconds
        {
            return false;
        }
        if (IR.decode())
        {
            if (c == 1)
            {
                che &= var[1] == IR.decodedIRData.decodedRawData; // check if the second third input match
                lcd.print("*");                                   // check if the second code is correct
            }
            else if (c == 2)
            {
                che &= var[2] == IR.decodedIRData.decodedRawData;
                lcd.print("*"); // check if the third code is correct
            }
            else if (c == 3)
            {
                che &= var[3] == IR.decodedIRData.decodedRawData;
                lcd.print("*"); // check if the fourth code is correct
            }
            delay(200); // wait for 0.3 seconds between each press
            c++;        // increment c
            IR.resume();
        }
    }
    if (che)
    { // if the passwords match update the password
        password[0] = var[0];
        password[1] = var[1];
        password[2] = var[2];
        password[3] = var[3];
    }
    IR.resume();
    return che;
}

bool check()
{
    return IR.decodedIRData.decodedRawData == button0 || IR.decodedIRData.decodedRawData == button1 || IR.decodedIRData.decodedRawData == button2 || IR.decodedIRData.decodedRawData == button3 || IR.decodedIRData.decodedRawData == button4 || IR.decodedIRData.decodedRawData == button5 || IR.decodedIRData.decodedRawData == button6 || IR.decodedIRData.decodedRawData == button7 || IR.decodedIRData.decodedRawData == button8 || IR.decodedIRData.decodedRawData == button9;
}