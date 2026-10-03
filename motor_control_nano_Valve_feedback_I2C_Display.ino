#include <SoftwareSerial.h>

#include <Wire.h>

SoftwareSerial nano_esp01_serial(10,11);  // 10 = Rx , 11 = Tx


#include<MedianFilter.h>
#include <LiquidCrystal_I2C.h>
//const int rs=12,en=13,d4=6,d5=7,d6=8,d7=9;
LiquidCrystal_I2C lcd(0x27,20,4);     //////// A4 = SDA, A5 = SCL
//LiquidCrystal_I2C lcd(0x3F,20,4);


int tank1height = 119;    // Tank 1 Height
int tank1percent;
double tank1waterheight;
long tank1distance;

int tank2height = 119;    // Tank 2 Height
int tank2percent;
double tank2waterheight;
long tank2distance;

double durationtank1;
double durationtank2;

long tank1distance_raw;
long tank2distance_raw;

long distance;
double duration;

const int trigpin1= A0;          // tank 1 trig pin output
const int echopin1= A1;          // tank 1 echo pin input
const int trigpin2= A2;          // tank 2 trig pin output
const int echopin2= A3;          // tank 2 echo pin input

int motorstatus1;
int motorstatus2;
int motorrelay = 2;   // Motor Relay Output pin

int valve1;
const int valverelay1 = 6;  // Tank 1 Valve Relay Output pin

int valve2;
const int valverelay2 = 7;  // Tank 2 Valve Relay Output pin






int motordelay = 30000;   // valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )

int motoronpercent = 50;           // Motor ON Percent

int motoroffpercent_tank1_auto = 75;          // Motor OFF Percent Auto
int motoroffpercent_tank2_auto = 85;

int motoroffpercent_tank1_manual = 80;        // Motor OFF Percent Manual
int motoroffpercent_tank2_manual = 85;

int tank1_maximum_level_show = 80;
int tank2_maximum_level_show = 85;




const int tank1manualpin = 4;   // Tank 1 Manual Switch Input pin
const int tank2manualpin = 5;   // Tank 2 Manual Switch Input pin

int tank1manual = 0;
int tank2manual = 0;

const int powerstatuspin = A6;     // AC Power Status input pin
int powerstatus;

const int valve_feedback_common_input_pin = A7;   // v/v feedback input pin
int valve_feedback_common = 0;
int valve_1_feedback = 0;
int valve_2_feedback = 0;


const int pushonpin = 3;   // Manual Push Switch Input Pin
int pushstate = 0;
int pushon = 0;

char pc_serial_input;

unsigned long timecount_feedback_current;
unsigned long timecount_feedback_start;

unsigned long timecount_powerstatus_current;
unsigned long timecount_powerstatus_start;

///////////////////////////////////////////////// serial data variable for ESP01 //////////////////////////////////////
int sdata1 = 0;
int sdata2 = 0;
int sdata3 = 0;
int sdata4 = 0;
int sdata5 = 0;
int sdata6 = 0;
int sdata7 = 0;
int sdata8 = 0;
int sdata9 = 0;
int sdata10 = 0;
int sdata11 = 0;
int sdata12 = 0;
int sdata13 = 0;
int sdata14 = 0;
int sdata15 = 0;
int sdata16 = 0;
int sdata17 = 0;


String cdata;

long int data;

int motorstatus;
int tank1manualstatus;
int tank2manualstatus;

int localremotefeedback = 201;
int localremotefeedbackflag=0;
int tank1swfeedback;
int tank1swfeedbackflag=0;
int tank2swfeedback;
int tank2swfeedbackflag=0;



int localremote = 0;
int tank1manualremote = 1;
int tank2manualremote = 1;
int pushonremote = 0;
int motoronalarm;

int acvoltage;
int aclowvoltage;
int aclowvoltagebypass;
int aclowvoltagevalue = 170;
///////////////////////////////////////////////// serial data variable for ESP01 finished //////////////////////////////////////


////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////


void showtank()
{
  lcd.setCursor(0, 0);         // Tank1
  lcd.print("Tank - 1 level: ");
  lcd.print(tank1percent);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("  Water Depth: ");
  lcd.print(tank1distance);
  lcd.print("cm");
  
  
  lcd.setCursor(0, 2);         // Tank2
  lcd.print("Tank - 2 level: ");
  lcd.print(tank2percent);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("  Water Depth: ");
  lcd.print(tank2distance);
  lcd.print("cm");


  /*lcd.clear();             // AC Voltage
  lcd.setCursor(0, 0);
  lcd.print(" *** AC Voltage *** ");
  lcd.setCursor(7, 1);
  lcd.print(acvoltage);*/
}


void showtank1empty()
{
  //lcd.blink();
  lcd.setCursor(0,0);        //tank1
  lcd.print(" * Tank - 1 Empty * ");
  
  lcd.setCursor(0, 2);       //tank2
  lcd.print("Tank - 2 level: ");
  lcd.print(tank2percent);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("  Water Depth: ");
  lcd.print(tank2distance);
  lcd.print("cm");
}



void showtank1motoron()
{
  //lcd.blink();
  lcd.setCursor(0,0);        //tank1
  lcd.print(" * Motor  Running * ");
  
  lcd.setCursor(0,1);
  lcd.print("* Tank1 Valve Open *");
  //delay(50);
  //lcd.noBlink();
  
  lcd.setCursor(0, 2);       //tank2
  lcd.print("Tank - 2 level: ");
  lcd.print(tank2percent);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("  Water Depth: ");
  lcd.print(tank2distance);
  lcd.print("cm");
}


void showtank1full()
{
  //lcd.blink();
  lcd.setCursor(0,0);    //tank1

  lcd.print(" * Tank - 1  Full * ");
  //delay(50);
  //lcd.noBlink();
  
  lcd.setCursor(0, 2);       //tank2
  lcd.print("Tank - 2 level: ");
  lcd.print(tank2percent);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("  Water Depth: ");
  lcd.print(tank2distance);
  lcd.print("cm");
}


void showtank2empty()
{
  //lcd.blink();
  lcd.setCursor(0, 0);       //tank1
  lcd.print("Tank - 1 level: ");
  lcd.print(tank1percent);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("  Water Depth: ");
  lcd.print(tank1distance);
  lcd.print("cm");
  
  lcd.setCursor(0,2);        //tank2
  lcd.print(" * Tank - 2 Empty * ");
  //delay(50);
  //lcd.noBlink();  
}



void showtank2motoron()
{
  //lcd.blink();
  lcd.setCursor(0, 0);       //tank1
  lcd.print("Tank - 1 level: ");
  lcd.print(tank1percent);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("  Water Depth: ");
  lcd.print(tank1distance);
  lcd.print("cm");
  
  lcd.setCursor(0,2);        //tank2
  lcd.print(" * Motor  Running * ");
  
  lcd.setCursor(0,3);
  lcd.print("* Tank2 Valve Open *");
  //delay(50);
  //lcd.noBlink();
}


void showtank2full()
{
  //lcd.blink();
  lcd.setCursor(0, 0);       //tank1
  lcd.print("Tank - 1 level: ");
  lcd.print(tank1percent);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("  Water Depth: ");
  lcd.print(tank1distance);
  lcd.print("cm");
  
  lcd.setCursor(0,2);        //tank2
  lcd.print(" * Tank - 2  Full * ");
  //delay(50);
  //lcd.noBlink();
}

void showtank1valve1()
{
  lcd.setCursor(0, 0);         //tank1
  lcd.print("Tank - 1 level: ");
  lcd.print(tank1percent);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("  Water Depth: ");
  lcd.print(tank1distance);
  lcd.print("cm");
  
  lcd.setCursor(0, 1);
  lcd.print("* Tank1 Valve Open *");
  //delay(200);
  
  
  lcd.setCursor(0, 2);         //tank2
  lcd.print("Tank - 2 level: ");
  lcd.print(tank2percent);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("  Water Depth: ");
  lcd.print(tank2distance);
  lcd.print("cm");
}


void showtank2valve2()
{
  lcd.setCursor(0, 0);         //tank1
  lcd.print("Tank - 1 level: ");
  lcd.print(tank1percent);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("  Water Depth: ");
  lcd.print(tank1distance);
  lcd.print("cm");
  
  lcd.setCursor(0, 2);         //tank2
  lcd.print("Tank - 2 level: ");
  lcd.print(tank2percent);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("  Water Depth: ");
  lcd.print(tank2distance);
  lcd.print("cm");
  
  lcd.setCursor(0, 3);
  lcd.print("* Tank2 Valve Open *");
  //delay(200);
}


void showtank1manual()
{
  lcd.setCursor(0, 0);         //tank1
  lcd.print("Tank - 1 level: ");
  lcd.print(tank1percent);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("  Water Depth: ");
  lcd.print(tank1distance);
  lcd.print("cm");
  delay(500);
  
  lcd.setCursor(0, 1);
  lcd.print(" ** Tank - 1 OFF ** ");
  //delay(200);
  
  
  lcd.setCursor(0, 2);         //tank2
  lcd.print("Tank - 2 level: ");
  lcd.print(tank2percent);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("  Water Depth: ");
  lcd.print(tank2distance);
  lcd.print("cm"); 
}

void showtank2manual()
{
  lcd.setCursor(0, 0);         //tank1
  lcd.print("Tank - 1 level: ");
  lcd.print(tank1percent);
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("  Water Depth: ");
  lcd.print(tank1distance);
  lcd.print("cm");
  
  lcd.setCursor(0, 2);         //tank2
  lcd.print("Tank - 2 level: ");
  lcd.print(tank2percent);
  lcd.print("%");
  
  lcd.setCursor(0, 3);
  lcd.print("  Water Depth: ");
  lcd.print(tank2distance);
  lcd.print("cm");
  delay(500);
  
  lcd.setCursor(0, 3);
  lcd.print(" ** Tank - 2 OFF ** ");
  //delay(200); 
}



void showlocalremote()
{
  lcd.setCursor(0, 1);         
  lcd.print("** Remote Control **");
  lcd.setCursor(0, 2);         
  lcd.print(" **** Running ****  ");
}


void powerfail()
{
  lcd.setCursor(0, 1);         
  lcd.print(" *** Power Fail *** "); 
}


/*void showvoltage()
{
  lcd.setCursor(0, 0);
  lcd.print(" *** AC Voltage *** ");
  lcd.setCursor(7, 2);
  lcd.print(acvoltage);
}

void lowvoltpowerfail()
{
  lcd.setCursor(0, 0);
  lcd.print(" *** AC Voltage *** ");
  lcd.setCursor(7, 1);
  lcd.print(acvoltage);
  lcd.setCursor(0, 2);
  lcd.print("Low Volt , Motor OFF");
  lcd.setCursor(0, 3);
  lcd.print("        *****       ");
}*/


//////////////////////////////////////////////////////// Sonar Sensor Function //////////////////////////////////////////////////////////////////////////////////

void SonarSensor(int trigPinSensor,int echoPinSensor)      //it takes the trigPIN and the echoPIN
{
  digitalWrite(trigPinSensor, LOW);// put trigpin LOW 
  delayMicroseconds(2);// wait 2 microseconds
  digitalWrite(trigPinSensor, HIGH);// switch trigpin HIGH
  delayMicroseconds(10); // wait 10 microseconds
  digitalWrite(trigPinSensor, LOW);// turn it LOW again

  duration = pulseIn(echoPinSensor, HIGH);//pulseIn funtion will return the time on how much the configured pin remain the level HIGH or LOW; in this case it will return how much time echoPinSensor stay HIGH
  distance= (duration/2) / 29.1; // first we have to divide the duration by two
}

//////////////////////////////////////////////////////// Sonar Sensor Function /////////////////////////////////////////////////////////////////////////////////////



MedianFilter filter1(6,0);
MedianFilter filter2(7,0);




void setup()      ////////////////////// void setup ////////////////////////       
{
 
 
 lcd.begin(); 
 
 Serial.begin(9600);
 nano_esp01_serial.begin(9600);
 
 pinMode(trigpin1,OUTPUT);
 pinMode(trigpin2,OUTPUT);
 pinMode(echopin1,INPUT);
 pinMode(echopin2,INPUT);
 
 
 pinMode(valverelay1,OUTPUT);
 pinMode(valverelay2,OUTPUT);
 
 pinMode(motorrelay,OUTPUT);
 
 
 
 pinMode(pushonpin,INPUT_PULLUP);
 pinMode(tank1manualpin,INPUT);
 pinMode(tank2manualpin,INPUT);

 //pinMode(powerstatuspin,INPUT);


 
 timecount_powerstatus_current=millis();


 
}   ////////////////////// void setup finished ////////////////////////


void loop()             ////////////////////////////////////////////////////////////////// void loop //////////////////////////////////////////////////////////////
{ 


timecount_feedback_current=millis();

/*
/////////////////////////////////////////////////////////// simulation through pc serial ///////////////////////
//tank1percent=60;             // Simulation Mode
//tank2percent=55;             // Simulation Mode


if(Serial.available()!= 0)
{
   pc_serial_input = Serial.read();
}
if(pc_serial_input=='1')
{
  tank1percent= 75;
}
if(pc_serial_input=='2')
{
  tank2percent= 85;
}

if(pc_serial_input=='3')
{
  tank1percent= 60;
}
if(pc_serial_input=='4')
{
  tank2percent= 65;
}
if(pc_serial_input=='5')
{
  tank2percent= 55;
}
if(pc_serial_input=='0')
{
  pushstate=HIGH;
}

if(pc_serial_input=='6')
{
  valve_1_feedback=HIGH;
  valve_2_feedback = LOW;
}
if(pc_serial_input=='7')
{
  valve_2_feedback=HIGH;
  valve_1_feedback = LOW;
}

if(pc_serial_input=='8')
{
  valve_1_feedback = LOW;
}
if(pc_serial_input=='9')
{
  valve_2_feedback = LOW;
}

////////////////////////////////////////////////////////////
*/
  
  ///////////////////////////////////////////////////////////// Simulation Mode////////////////////////////////////
  
  /*int demovalue1 = analogRead(echopin1);  //set the depth of your tank by using the potentiometer
  int value1 = map(demovalue1, 0, 1023, 0, tank1height);  //convert 0 to 1023 into 0 to tank1height = 119 cm
  tank1distance = value1;
  
  int demovalue2 = analogRead(echopin2);  //set the depth of your tank by using the potentiometer
  int value2 = map(demovalue2, 0, 1023, 0, tank2height);  //convert 0 to 1023 into 0 to tank2height = 119 cm
  tank2distance = value2;*/
  
  ///////////////////////////////////////////////////////////// Simulation Mode////////////////////////////////////
  


  
  
  
  //////////////////////////////////////////////////////// Sonar Sensor Function with Median Filter //////////////////////////////////

  
  SonarSensor(trigpin1,echopin1);              // look void SonarSensor(int trigPinSensor,int echoPinSensor) for SonarSensor function
  tank1distance_raw = distance;                      // store the distance in the first variable
  SonarSensor(trigpin2,echopin2);               // call the SonarSensor function again with the second sensor pins
  tank2distance_raw = distance;                      // store the new distance in the second variable

  unsigned int tank1distance_filter_out,tank1distance_filter_in=tank1distance_raw;
  filter1.in(tank1distance_filter_in);
  tank1distance_filter_out=filter1.out();

  unsigned int tank2distance_filter_out,tank2distance_filter_in=tank2distance_raw;
  filter2.in(tank2distance_filter_in);
  tank2distance_filter_out=filter2.out();


  tank1distance=tank1distance_filter_out;
  tank2distance=tank2distance_filter_out;

  //////////////////////////////////////////////////////// Sonar Sensor Function with Median Filter finished //////////////////////////////////




  /////////////////////////////////////////////////////// AC Power Status Check ////////////////////////////////////

  int powerstatusvalue=analogRead(powerstatuspin);     /////// Motor Power Status /////////
  if(powerstatusvalue>500 && powerstatusvalue<=1023)
  {
    powerstatus=HIGH;

/*    
    timecount_powerstatus_start=millis();

    Serial.print("timecount_powerstatus_start: ");
            Serial.println(timecount_powerstatus_start);
      Serial.print("timecount_powerstatus_current: ");
            Serial.println(timecount_powerstatus_current);
      Serial.print("timecount_powerstatus: ");
            Serial.println((timecount_powerstatus_start - timecount_powerstatus_current)/1000);
    

    if((timecount_powerstatus_start - timecount_powerstatus_current) >= 60000)         // AC Power comeback. System will be normal after 1 minute
    {
      powerstatus=HIGH;
    }
    else
    {
      powerstatus=LOW;
    }

*/    
  }



  if(powerstatusvalue<500)
  {
    powerstatus=LOW;
  }


        //powerstatus=HIGH;     // Simulation for Motor Control, Magnetic Contactor power supply Circuit Breaker Must off this time.


        

//////////////////////////////////////////// ac voltage ///////////////////////////////////

          
          
          
          
          acvoltage=220;

/////////////////////////////////////////////// ac low voltage /////////////////////////////

if(aclowvoltagebypass==LOW)
{
  if(acvoltage <= aclowvoltagevalue)
  {
    aclowvoltage=LOW;     // ac voltage Low
  }

  if(acvoltage > aclowvoltagevalue)
  {
    aclowvoltage=HIGH;     // ac voltage High
  }
}

if(aclowvoltagebypass==HIGH)
{
  aclowvoltage=HIGH;     // AC Low Voltage Bypass
}

  
  
  /////////////////////////////////////////////////////// AC Power Status Check finished ////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////// Valve Feedback ////////////////////////////////////////////

  int valve_feedback_common=analogRead(valve_feedback_common_input_pin);
  //Serial.print("valve_feedback_common: ");
  //Serial.println(valve_feedback_common);


  //valve_feedback_common = 801; ////// Simulation , 801 = v/v-1, 301 = v/v-2

  //////////////////////////// v/v - 1 feedback //////////////
  
  if(valve_feedback_common > 800 && valve_feedback_common <= 1023)
  {
    valve_1_feedback = HIGH;
    //Serial.print("valve_feedback_common: ");
    //Serial.println(valve_feedback_common);
    Serial.println("valve_1_feedback: High & Open");
  }
  else
  {
    valve_1_feedback = LOW;
  }

  

  ///////////////////////////// v/v - 2 feedback //////////////
  
  if(valve_feedback_common > 300 && valve_feedback_common <= 520)
  {
    valve_2_feedback = HIGH;
    //Serial.print("valve_feedback_common: ");
    //Serial.println(valve_feedback_common);
    Serial.println("valve_2_feedback: High & Open");
  }
  else
  {
    valve_2_feedback = LOW;
  }

  //////////////////////////////////////////////////////////////////////////// Valve Feedback Finished ////////////////////////////////////////////

  
  //////////////////////////////////////////////////// Serial Print Display ////////////////////////////////////////
  
  
  /*Serial.print("powerstatusvalue: ");
  Serial.println(powerstatusvalue);
  Serial.print("powerstatus: ");
  Serial.println(powerstatus);
  
  
  Serial.print("tank1manual: ");
  Serial.println(tank1manual);
  Serial.print("tank2manual: ");
  Serial.println(tank2manual);
  
  Serial.print("pushstate: ");
  Serial.println(pushstate);

  Serial.print("motorstatus1: ");
  Serial.println(motorstatus1);

  Serial.print("motorstatus2: ");
  Serial.println(motorstatus2);
  
  Serial.print("Tank-1 Percent: ");
  Serial.println(tank1percent);

  Serial.print("tank1distance: ");
  Serial.println(tank1distance);
  
  Serial.print("Tank-2 Percent: ");
  Serial.println(tank2percent);

  Serial.print("tank2distance: ");
  Serial.println(tank2distance);*/

  //////////////////////////////////////////////////// Serial Print Display ////////////////////////////////////////
  

  
  
  pushon=digitalRead(pushonpin);
  tank1manual=digitalRead(tank1manualpin);
  tank2manual=digitalRead(tank2manualpin);
  
  
  
  
  void showtank();
  
  void showtank1empty();
  void showtank1motoron();
  void showtank1full();
  
  void showtank2empty();
  void showtank2motoron();
  void showtank2full();
  
  void showtank1valve1();
  void showtank2valve2();
  
  void showtank1manual();
  void showtank2manual();
  
  void showlocalremote();
  
  void powerfail();

  void showvoltage();
  void lowvoltpowerfail();
  
  
  lcd.clear();
  

  /////////////////////////////////////////////////////// Ultrasonic Distance Without Filter ///////////////////////////////////
  
  /*digitalWrite(trigpin1,LOW);
  delayMicroseconds(1);
  digitalWrite(trigpin1,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin1,LOW);
  durationtank1=pulseIn(echopin1,HIGH);
  tank1distance=(durationtank1 * 0.034) / 2;
  
  digitalWrite(trigpin2,LOW);
  delayMicroseconds(1);
  digitalWrite(trigpin2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin2,LOW);
  durationtank2=pulseIn(echopin2,HIGH);
  tank2distance=(durationtank2 * 0.034) / 2;*/

  /////////////////////////////////////////////////////// Ultrasonic Distance Without Filter finished ///////////////////////////////////
  



  /////////////////////////////////////////////////////// Ultrasonic Distance With Median Filter ///////////////////////////////////
  
  /*digitalWrite(trigpin1,LOW);
  delayMicroseconds(1);
  digitalWrite(trigpin1,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin1,LOW);
  durationtank1=pulseIn(echopin1,HIGH);
  tank1distance_raw=(durationtank1 * 0.034) / 2;
  
  digitalWrite(trigpin2,LOW);
  delayMicroseconds(1);
  digitalWrite(trigpin2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin2,LOW);
  durationtank2=pulseIn(echopin2,HIGH);
  tank2distance_raw=(durationtank2 * 0.034) / 2;


  unsigned int tank1distance_filter_out,tank1distance_filter_in=tank1distance_raw;
  filter1.in(tank1distance_filter_in);
  tank1distance_filter_out=filter1.out();

  unsigned int tank2distance_filter_out,tank2distance_filter_in=tank2distance_raw;
  filter2.in(tank2distance_filter_in);
  tank2distance_filter_out=filter2.out();


  tank1distance=tank1distance_filter_out;
  tank2distance=tank2distance_filter_out;*/

  /////////////////////////////////////////////////////// Ultrasonic Distance With Median Filter finished ///////////////////////////////////
  

  
  
  
  /////////////////////////////////////////////////////// Percent Calculation /////////////////////////////////////////////////////////
  
  tank1waterheight = tank1height - tank1distance;
  tank1percent = ((tank1waterheight / tank1height) * 100) + 10 ;        //  10% to avoid over flow and prevent sensor from water, tank 1
  
  tank2waterheight = tank2height - tank2distance;
  tank2percent = ((tank2waterheight / tank2height) * 100) + 10 ;        //  10% to avoid over flow and prevent sensor from water, tank 2

  /////////////////////////////////////////////////////// Percent Calculation finished /////////////////////////////////////////////////////////


  
  //tank1percent= 65;
  //tank2percent= 67;

  
  
  if (tank1percent > 100)      //tank 1 condition
  {
    tank1percent = 100;
  }
  if (tank1percent < 0)
  {
    tank1percent = 0;
  }
  
  
  
  if (tank2percent > 100)      //tank 2 condition 
  {
    tank2percent = 100;
  }
  if (tank2percent < 0)
  {
    tank2percent = 0;
  }
  
  
  
  
  if (tank1percent <= 100 && tank2percent <= 100 )       //tank condition for LCD Display
    { 
      lcd.clear();
      showtank();
      delay(100);
      
      /*lcd.clear();
      showvoltage();
      delay(500);*/
      
      if ( motorstatus1==HIGH)    // tank 1 condition
      { 
        lcd.clear();
        showtank();
        delay(1000);
        lcd.clear();
        showtank1motoron();
        delay(1000); 
      }
      
      if (( tank1percent >= tank1_maximum_level_show) && ( tank1manual==HIGH && tank2manual==HIGH ))
      { 
        lcd.clear();
        showtank();
        delay(1000);
        lcd.clear();
        showtank1full();
        delay(1000); 
      }
      if (( tank1percent >= tank1_maximum_level_show) && ( tank1manual==HIGH && tank2manual==LOW ))
      { 
        lcd.clear();
        showtank();
        delay(1000);
        lcd.clear();
        showtank1full();
        delay(1000); 
      }
      
      if ( valve_1_feedback==HIGH && motorstatus1==LOW)
      { 
        lcd.clear();
        showtank();    
        delay(1000);
        lcd.clear();
        showtank1valve1();
        delay(1000); 
      } 
      
      if( tank1manual==LOW && localremote==LOW )  // Local Mode
      {
        lcd.clear();
        showtank1manual();    
        delay(1000);
      }

      if( tank1manualremote==LOW && localremote==HIGH )  // Remote App Mode
      {
        lcd.clear();
        showtank1manual();    
        delay(1000);
      }
      
      if( tank1percent <= 10 )
      {
        lcd.clear();
        showtank1empty();
        delay(1000);
      }
    


      
      if ( motorstatus2==HIGH)       // tank 2 condition
      { 
        lcd.clear();
        showtank();    
        delay(1000);
        lcd.clear();
        showtank2motoron();
        delay(1000); 
      }
      
      if (( tank2percent >= tank2_maximum_level_show) && ( tank1manual==HIGH && tank2manual==HIGH ))
      { 
        lcd.clear();
        showtank();   
        delay(1000);
        lcd.clear();
        showtank2full();
        delay(1000); 
      }
      if (( tank2percent >= tank2_maximum_level_show) && ( tank1manual==LOW && tank2manual==HIGH ))
      { 
        lcd.clear();
        showtank();   
        delay(1000);
        lcd.clear();
        showtank2full();
        delay(1000); 
      }
      
      if ( valve_2_feedback==HIGH && motorstatus2==LOW)
      { 
        lcd.clear();
        showtank();    
        delay(1000);
        lcd.clear();
        showtank2valve2();
        delay(1000); 
      }
      
      if( tank2manual==LOW && localremote==LOW )  // Local Mode
      {
        lcd.clear();
        showtank2manual();    
        delay(1000);
      }

      if( tank2manualremote==LOW && localremote==HIGH )  // Remote App Mode
      {
        lcd.clear();
        showtank2manual();    
        delay(1000);
      }
      
      if( tank2percent <= 10 )
      {
        lcd.clear();
        showtank2empty();
        delay(1000);
      }

      
      
      if(powerstatus==0)    // power status
      {
        lcd.clear();
        showtank();
        delay(1000);
        lcd.clear();
        powerfail();
        delay(1000);
      }

      

       if(localremote == HIGH)   // Local(Arduino) / Remote(Blynk App)
       {
        lcd.clear();
        showtank();
        delay(1000);
        lcd.clear();
        showlocalremote();
        delay(1000);
        }


        /*if(aclowvoltage == LOW)      // ac low voltage power fail
        {
          lcd.clear();
          showtank();
          delay(1000);
          lcd.clear();
          lowvoltpowerfail();
          delay(1000);
        }*/
      
    }
    








    
if ( localremote == LOW )      ///////////////////////////////// Local Mode Condition ///////////////////////////////////////////
{
    
///////////////////////////////////////////////////////////////////////// Auto Mode ////////////////////////////////////////////////////////////////////////
    
    if( tank1manual==HIGH && tank2manual==HIGH )   // tank1 and tank2 auto for all manual switch HIGH
    {
      if(tank1percent < tank2percent && motorstatus2==LOW && tank1manual==HIGH && (tank1percent!=100 && tank2percent!=100) /*&& tank1percent <= 70*/)      //tank 1 condition valve1 on
      {
        
        valve1=HIGH; 
        //digitalWrite(valverelay1, valve1);
        Serial.println("valve 1 Relay open");
        valve2=LOW;
        //digitalWrite(valverelay2, valve2);
      }
      
      
      
      
      if(tank2percent <= tank1percent && motorstatus1==LOW && tank2manual==HIGH && (tank1percent!=100 && tank2percent!=100) )      //tank 2 condition valve2 on
      {
        
        valve2=HIGH;
        //digitalWrite(valverelay2, valve2);
        Serial.println("valve 2 Relay open");
        valve1=LOW;
        //digitalWrite(valverelay1, valve1);
      }





      digitalWrite(valverelay1, valve1);
      digitalWrite(valverelay2, valve2);

      
      
     
      
      //// ON Condition Tank-1
      
      if((valve1==HIGH && valve_1_feedback==HIGH) && (valve2==LOW && valve_2_feedback==LOW) && (powerstatus==HIGH && aclowvoltage==HIGH) && tank1percent<=motoronpercent )            //tank 1 condition motor on
      {
      
      delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000, 1 minuit = 60000 )
      digitalWrite(motorrelay, HIGH);
      motorstatus1=HIGH;
      
      Serial.println("Tank-1 Motor ON");
      }
      
      if((valve1==HIGH && valve_1_feedback==HIGH) && (valve2==LOW && valve_2_feedback==LOW) && (powerstatus==HIGH && aclowvoltage==HIGH) && (pushon==HIGH || pushstate==HIGH) && tank1percent<=motoroffpercent_tank1_auto)            //tank 1 condition motor on manually
      {
      
      pushstate=HIGH;
      Serial.print("pushstate: ");
      Serial.println(pushstate);
      

      digitalWrite(motorrelay, HIGH);
      motorstatus1=HIGH;
      Serial.println("Tank-1 Motor ON Manually");      
      }


      

      //// ON Condition Tank-2
      
      if((valve2==HIGH && valve_2_feedback==HIGH) && (valve1==LOW && valve_1_feedback==LOW) && (powerstatus==HIGH && aclowvoltage==HIGH) && tank2percent<=motoronpercent)            //tank 2 condition motor on
      {
      
      delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )
      digitalWrite(motorrelay, HIGH);
      motorstatus2=HIGH;
      
      Serial.println("Tank-2 Motor ON");
      }
      
      if((valve2==HIGH && valve_2_feedback==HIGH) && (valve1==LOW && valve_1_feedback==LOW) && (powerstatus==HIGH && aclowvoltage==HIGH) && (pushon==HIGH ||  pushstate==HIGH) && tank2percent<=motoroffpercent_tank2_auto)            //tank 2 condition motor on manually
      {
      
      pushstate=HIGH;
      Serial.print("pushstate: ");
      Serial.println(pushstate);
      
      digitalWrite(motorrelay, HIGH);
      motorstatus2=HIGH;
      
      Serial.println("Tank-2 Motor ON Manually");
      }

      

      //// OFF Condition Tank-1
      
      if(valve1==HIGH && motorstatus1==HIGH && (tank1percent>=motoroffpercent_tank1_auto || valve_1_feedback==LOW || tank1manual==LOW || (powerstatus==LOW || aclowvoltage==LOW) ) )       //tank 1 condition motor off
      {
        //timecount_feedback_current=millis;
        Serial.print("timecount_feedback_current_1: ");
        Serial.println(timecount_feedback_current);
        
        if(powerstatus==LOW)
        {
          digitalWrite(motorrelay, LOW);
          motorstatus1=LOW;
          
          pushstate=LOW;
          
          Serial.println("Motor OFF Due to Main Power Failure");
        }

        else if(valve1==HIGH && valve_1_feedback==LOW)
        {
          //timecount_feedback_current=millis();

          if(timecount_feedback_current >= (timecount_feedback_start + 20000))
          {
            Serial.print("v/v change timer count: ");
            Serial.println((timecount_feedback_current - timecount_feedback_start)/1000);
            
            if(valve_1_feedback==LOW)
            {
              digitalWrite(motorrelay, LOW);
              motorstatus1=LOW;
          
              pushstate=LOW;
          
              Serial.println("Motor OFF Due to V/V Not Operate");
            }
            else 
            {
              motorstatus1=HIGH;
              Serial.println("V/V Operate Succesfully");
            }
          }
          
          
        }
          

        else if( tank2percent < motoroffpercent_tank2_auto && tank2manual==HIGH )
        {
          valve1=LOW;
          //digitalWrite(valverelay1, valve1);
          valve2=HIGH;
          //digitalWrite(valverelay2, valve2);

          motorstatus1=LOW;
          motorstatus2=HIGH;

          pushstate=LOW;

          timecount_feedback_start=millis();
        }

        else
        {
          digitalWrite(motorrelay, LOW);
          motorstatus1=LOW;

          pushstate=LOW;
        }

      }
      
      
      
      
      
      
      
      //// OFF Condition Tank-2
      
      if(valve2==HIGH && motorstatus2==HIGH && (tank2percent>=motoroffpercent_tank2_auto || valve_2_feedback==LOW || tank2manual==LOW || (powerstatus==LOW || aclowvoltage==LOW) ) )        //tank 2 condition motor off
      {
        //timecount_feedback_current=millis;
        Serial.print("timecount_feedback_current_2: ");
        Serial.println(timecount_feedback_current);
        
        if(powerstatus==LOW)
        {
          digitalWrite(motorrelay, LOW);
          motorstatus1=LOW;
          
          pushstate=LOW;
          
          Serial.println("Motor OFF Due to Main Power Failure");
        }

        else if(valve2==HIGH && valve_2_feedback==LOW)
        {
          //timecount_feedback_current=millis();

          if(timecount_feedback_current >= (timecount_feedback_start + 20000))
          {
            Serial.print("v/v change timer count: ");
            Serial.println((timecount_feedback_current - timecount_feedback_start)/1000);
            if(valve_2_feedback==LOW)
            {
              digitalWrite(motorrelay, LOW);
              motorstatus2=LOW;
          
              pushstate=LOW;
          
              Serial.println("Motor OFF Due to V/V Not Operate");
            }
            else 
            {
              motorstatus2=HIGH;
              Serial.println("V/V Operate Succesfully");
            }
          }
          
        }
          

        else if( tank1percent < motoroffpercent_tank1_auto && tank1manual==HIGH )
        {
          valve2=LOW;
          //digitalWrite(valverelay2, valve2);
          valve1=HIGH;
          //digitalWrite(valverelay1, valve1);

          motorstatus2=LOW;
          motorstatus1=HIGH;

          pushstate=LOW;

          timecount_feedback_start=millis();
        }

        else
        {
          digitalWrite(motorrelay, LOW);
          motorstatus2=LOW;

          pushstate=LOW;
        }

      }
    }    
     

///////////////////////////////////////////////////////////////////////// Auto Mode Finished ////////////////////////////////////////////////////////////////////////
    
    
 //////////////////////////////////////////////////////////////////////////// manual mode ////////////////////////////////////////////////////////////////
    
    else           // auto mode, but only one tank is in service
    {

/////////////////////////////////////////////////////////////////// Manual Mode - Tank 1 OFF /////////////////////////////////////////     

      
      if(tank1manual==LOW )         // tank 1 low, that time tank 2 is in service
      {
        if(motorstatus1==HIGH)
        {
          digitalWrite(motorrelay,LOW);
          motorstatus1=LOW;
          pushstate=LOW;
        }
        
        if(tank1manual==LOW && tank2manual==LOW)
        {
          valve1=LOW;
          valve2=LOW;
         pushstate=LOW; 
        }
        
        if(tank1manual==LOW && tank2manual==HIGH && tank2percent < 100)
        {
          valve1=LOW;
          valve2=HIGH;
          pushstate=LOW;
        }
          
        digitalWrite(valverelay1, valve1);
        digitalWrite(valverelay2, valve2);
     
        
     
        if(tank2manual==HIGH && (valve2==HIGH && valve_2_feedback==HIGH) && (powerstatus==HIGH && aclowvoltage==HIGH) && tank2percent <= motoronpercent)
        {
          delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )
          digitalWrite(motorrelay,HIGH);
          motorstatus2=HIGH;
          
          Serial.println("Tank-2 Motor ON");
        }
        
        if(tank2manual==HIGH && (valve2==HIGH && valve_2_feedback==HIGH) && (powerstatus==HIGH && aclowvoltage==HIGH) && (pushon==HIGH || pushstate==HIGH) && tank2percent<=motoroffpercent_tank2_manual)
        {
          pushstate=HIGH;
          Serial.print("pushstate: ");
          Serial.println(pushstate);
         
          digitalWrite(motorrelay,HIGH);
          motorstatus2=HIGH;
          
          Serial.println("Tank-2 Motor ON Manually");
        }
     
        if(tank2manual==LOW || tank2percent>=motoroffpercent_tank2_manual || valve_2_feedback==LOW || (powerstatus==LOW || aclowvoltage==LOW))
        {
          digitalWrite(motorrelay, LOW);
          motorstatus2=LOW;
          //valve2=LOW;
          //digitalWrite(valverelay2, valve2);
          pushstate=LOW;
          
          Serial.println("Tank-2 Motor OFF");
        } 
     
     }
     
/////////////////////////////////////////////////////////////////// Manual Mode - Tank 2 OFF /////////////////////////////////////////     
   
    if(tank2manual==LOW )           // tank 2 low, that time tank 1 is in service
       {
        if(motorstatus2==HIGH)
        {
          digitalWrite(motorrelay,LOW);
          motorstatus2=LOW;
          pushstate=LOW;
        }
        
        if(tank1manual==LOW && tank2manual==LOW)
        {
          valve1=LOW;
          valve2=LOW; 
          pushstate=LOW;
        }
        
        if(tank1manual==HIGH && tank2manual==LOW && tank1percent < 100)
        {
          valve1=HIGH;
          valve2=LOW;
          pushstate=LOW;
        }
      
        
        digitalWrite(valverelay2, valve2);
        digitalWrite(valverelay1, valve1);
     
        
        
     
        if(tank1manual==HIGH && (valve1==HIGH && valve_1_feedback==HIGH) && (powerstatus==HIGH && aclowvoltage==HIGH) && tank1percent <= motoronpercent  /*&& tank1distance>=motorontankdistance*/)
        {
          delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )
          digitalWrite(motorrelay,HIGH);
          motorstatus1=HIGH;
          
          Serial.println("Tank-1 Motor ON");
        }
        
        if(tank1manual==HIGH && (valve1==HIGH && valve_1_feedback==HIGH) && (powerstatus==HIGH && aclowvoltage==HIGH) && (pushon==HIGH || pushstate==HIGH) && tank1percent<=motoroffpercent_tank1_manual)
        {
          pushstate=HIGH;
          Serial.print("pushstate: ");
          Serial.println(pushstate);
          
          digitalWrite(motorrelay,HIGH);
          motorstatus1=HIGH;
          
          Serial.println("Tank-1 Motor ON Manually");
        }
     
        if(tank1manual==LOW || tank1percent>=motoroffpercent_tank1_manual || valve_1_feedback==LOW || (powerstatus==LOW || aclowvoltage==LOW))
        {
          digitalWrite(motorrelay, LOW);
          motorstatus1=LOW;
          //valve1=LOW;
          //digitalWrite(valverelay1, valve1);
          pushstate=LOW;
          
          Serial.println("Tank-1 Motor OFF");
        } 
       }
    }
    
    
//////////////////////////////////////////////////////////////////////////// manual mode Finished ////////////////////////////////////////////////////////////////


     
}       /////////////////////////////////////// Local Mode Condition Finished ////////////////////////////////////////////////////////////////////////////////




else if ( localremote == HIGH )         ///////////////////////////////// Remote Blynk App Mode ///////////////////////////////////////////////////
{
  

///////////////////////////////////////////////////////////////////////// Auto Mode ////////////////////////////////////////////////////////////////////////
    
    if( tank1manualremote==HIGH && tank2manualremote==HIGH )   // tank1 and tank2 auto for all manual switch HIGH
    {
      if(tank1percent < tank2percent && motorstatus2==LOW && tank1manualremote==HIGH && (tank1percent!=100 && tank2percent!=100))      //tank 1 condition valve1 on
      {
        
        valve1=HIGH; 
        //digitalWrite(valverelay1, valve1);
        Serial.println("valve 1 Relay open");
        valve2=LOW;
        //digitalWrite(valverelay2, valve2);
      }
      
      
      
      
      if(tank2percent <= tank1percent && motorstatus1==LOW && tank2manualremote==HIGH && (tank1percent!=100 && tank2percent!=100))      //tank 2 condition valve2 on
      {
        
        valve2=HIGH;
        //digitalWrite(valverelay2, valve2);
        Serial.println("valve 2 Relay open");
        valve1=LOW;
        //digitalWrite(valverelay1, valve1);
      }





      digitalWrite(valverelay1, valve1);
      digitalWrite(valverelay2, valve2);

      
      
     
      
      
      
      if((valve1==HIGH && valve_1_feedback==HIGH) && (valve2==LOW && valve_2_feedback==LOW) && (powerstatus==HIGH && aclowvoltage==HIGH) && tank1percent<=motoronpercent)            //tank 1 condition motor on
      {
      
      //delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000, 1 minuit = 60000 )
      digitalWrite(motorrelay, HIGH);
      motorstatus1=HIGH;
      
      Serial.println("Tank-1 Motor ON");
      }
      
      if((valve1==HIGH && valve_1_feedback==HIGH) && (valve2==LOW && valve_2_feedback==LOW) && (powerstatus==HIGH && aclowvoltage==HIGH) && (pushonremote==HIGH || pushstate==HIGH) && tank1percent<=motoroffpercent_tank1_auto)            //tank 1 condition motor on manually
      {
      
      pushstate=HIGH;
      pushonremote=LOW;
      Serial.print("pushstate: ");
      Serial.println(pushstate);
        
      digitalWrite(motorrelay, HIGH);
      motorstatus1=HIGH;
      
      Serial.println("Tank-1 Motor ON Manually");
      }
      
      if((valve2==HIGH && valve_2_feedback==HIGH) && (valve1==LOW && valve_1_feedback==LOW) && (powerstatus==HIGH && aclowvoltage==HIGH) && tank2percent<=motoronpercent)            //tank 2 condition motor on
      {
      
      //delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )
      digitalWrite(motorrelay, HIGH);
      motorstatus2=HIGH;
      
      Serial.println("Tank-2 Motor ON");
      }
      
      if((valve2==HIGH && valve_2_feedback==HIGH) && (valve1==LOW && valve_1_feedback==LOW) && (powerstatus==HIGH && aclowvoltage==HIGH) && (pushonremote==HIGH ||  pushstate==HIGH) && tank2percent<=motoroffpercent_tank2_auto)            //tank 2 condition motor on manually
      {
      
      pushstate=HIGH;
      pushonremote=LOW;
      Serial.print("pushstate: ");
      Serial.println(pushstate);
      
      digitalWrite(motorrelay, HIGH);
      motorstatus2=HIGH;
      
      Serial.println("Tank-2 Motor ON Manually");
      }

      
      ////////// OFF Condition
      
      if(valve1==HIGH && motorstatus1==HIGH && (tank1percent>=motoroffpercent_tank1_auto || valve_1_feedback==LOW || tank1manualremote==LOW || (powerstatus==LOW || aclowvoltage==LOW)))       //tank 1 condition motor off
      {
        //timecount_feedback_current=millis;
        Serial.print("timecount_feedback_current_1: ");
        Serial.println(timecount_feedback_current);
        
        if(powerstatus==LOW)
        {
          digitalWrite(motorrelay, LOW);
          motorstatus1=LOW;
          
          pushstate=LOW;
          
          Serial.println("Motor OFF Due to Main Power Failure");
        }

        else if(valve1==HIGH && valve_1_feedback==LOW)
        {
          //timecount_feedback_current=millis();

          if(timecount_feedback_current >= (timecount_feedback_start + 20000))
          {
            Serial.print("v/v change timer count: ");
            Serial.println((timecount_feedback_current - timecount_feedback_start)/1000);
            
            if(valve_1_feedback==LOW)
            {
              digitalWrite(motorrelay, LOW);
              motorstatus1=LOW;
          
              pushstate=LOW;
          
              Serial.println("Motor OFF Due to V/V Not Operate");
            }
            else 
            {
              motorstatus1=HIGH;
              Serial.println("V/V Operate Succesfully");
            }
          }
          
          
        }
          

        else if( tank2percent < motoroffpercent_tank2_auto && tank2manualremote==HIGH )
        {
          valve1=LOW;
          //digitalWrite(valverelay1, valve1);
          valve2=HIGH;
          //digitalWrite(valverelay2, valve2);

          motorstatus1=LOW;
          motorstatus2=HIGH;

          pushstate=LOW;

          timecount_feedback_start=millis();
        }

        else
        {
          digitalWrite(motorrelay, LOW);
          motorstatus1=LOW;

          pushstate=LOW;
        }
      }
      







      
      if(valve2==HIGH && motorstatus2==HIGH && (tank2percent>=motoroffpercent_tank2_auto || valve_2_feedback==LOW || tank2manualremote==LOW || (powerstatus==LOW || aclowvoltage==LOW)))        //tank 2 condition motor off
      {
        //timecount_feedback_current=millis;
        Serial.print("timecount_feedback_current_2: ");
        Serial.println(timecount_feedback_current);
        
        if(powerstatus==LOW)
        {
          digitalWrite(motorrelay, LOW);
          motorstatus1=LOW;
          
          pushstate=LOW;
          
          Serial.println("Motor OFF Due to Main Power Failure");
        }

        else if(valve2==HIGH && valve_2_feedback==LOW)
        {
          //timecount_feedback_current=millis();

          if(timecount_feedback_current >= (timecount_feedback_start + 20000))
          {
            Serial.print("v/v change timer count: ");
            Serial.println((timecount_feedback_current - timecount_feedback_start)/1000);
            if(valve_2_feedback==LOW)
            {
              digitalWrite(motorrelay, LOW);
              motorstatus2=LOW;
          
              pushstate=LOW;
          
              Serial.println("Motor OFF Due to V/V Not Operate");
            }
            else 
            {
              motorstatus2=HIGH;
              Serial.println("V/V Operate Succesfully");
            }
          }
          
        }
          

        else if( tank1percent < motoroffpercent_tank1_auto && tank1manualremote==HIGH )
        {
          valve2=LOW;
          //digitalWrite(valverelay2, valve2);
          valve1=HIGH;
          //digitalWrite(valverelay1, valve1);

          motorstatus2=LOW;
          motorstatus1=HIGH;

          pushstate=LOW;

          timecount_feedback_start=millis();
        }

        else
        {
          digitalWrite(motorrelay, LOW);
          motorstatus2=LOW;

          pushstate=LOW;
        }
      }
   
    
    }

///////////////////////////////////////////////////////////////////////// Auto Mode Finished ////////////////////////////////////////////////////////////////////////
       
    
 //////////////////////////////////////////////////////////////////////////// manual mode ////////////////////////////////////////////////////////////////
    
    else           // auto mode, but only one tank is in service
    {

/////////////////////////////////////////////////////////////////// Manual Mode - Tank 1 OFF /////////////////////////////////////////     

      
      if(tank1manualremote==LOW )         // tank 1 low, that time tank 2 is in service
      {
        if(motorstatus1==HIGH)
        {
          digitalWrite(motorrelay,LOW);
          motorstatus1=LOW;
          pushstate=LOW;
        }
        
        if(tank1manualremote==LOW && tank2manualremote==LOW)
        {
          valve1=LOW;
          valve2=LOW;
         pushstate=LOW; 
        }
        
        if(tank1manualremote==LOW && tank2manualremote==HIGH && tank2percent < 100)
        {
          valve1=LOW;
          valve2=HIGH;
          pushstate=LOW;
        }
          
        digitalWrite(valverelay1, valve1);
        digitalWrite(valverelay2, valve2);
     
        
     
        if(tank2manualremote==HIGH && (valve2==HIGH && valve_2_feedback==HIGH) && (powerstatus==HIGH && aclowvoltage==HIGH) && tank2percent <= motoronpercent)
        {
          //delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )
          digitalWrite(motorrelay,HIGH);
          motorstatus2=HIGH;
          
          Serial.println("Tank-2 Motor ON");
        }
        
        if(tank2manualremote==HIGH && (valve2==HIGH && valve_2_feedback==HIGH) && (powerstatus==HIGH && aclowvoltage==HIGH) && (pushonremote==HIGH || pushstate==HIGH) && tank2percent<=motoroffpercent_tank2_manual)
        {
          pushstate=HIGH;
          pushonremote=LOW;
          Serial.print("pushstate: ");
          Serial.println(pushstate);
          //delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )
          digitalWrite(motorrelay,HIGH);
          motorstatus2=HIGH;
          
          Serial.println("Tank-2 Motor ON Manually");
        }
     
        if(tank2manualremote==LOW || tank2percent>=motoroffpercent_tank2_manual || valve_2_feedback==LOW || (powerstatus==LOW || aclowvoltage==LOW))
        {
          digitalWrite(motorrelay, LOW);
          motorstatus2=LOW;
          //valve2=LOW;
          //digitalWrite(valverelay2, valve2);
          pushstate=LOW;
          
          Serial.println("Tank-2 Motor OFF");
        } 
     
     }
     
/////////////////////////////////////////////////////////////////// Manual Mode - Tank 2 OFF /////////////////////////////////////////     
   
    if(tank2manualremote==LOW )           // tank 2 low, that time tank 1 is in service
       {
        if(motorstatus2==HIGH)
        {
          digitalWrite(motorrelay,LOW);
          motorstatus2=LOW;
          pushstate=LOW;
        }
        
        if(tank1manualremote==LOW && tank2manualremote==LOW)
        {
          valve1=LOW;
          valve2=LOW; 
          pushstate=LOW;
        }
        
        if(tank1manualremote==HIGH && tank2manualremote==LOW && tank1percent < 100)
        {
          valve1=HIGH;
          valve2=LOW;
          pushstate=LOW;
        }
      
        
        digitalWrite(valverelay2, valve2);
        digitalWrite(valverelay1, valve1);
     
        
     
        if(tank1manualremote==HIGH && (valve1==HIGH && valve_1_feedback==HIGH) && (powerstatus==HIGH && aclowvoltage==HIGH) && tank1percent <= motoronpercent  /*&& tank1distance>=motorontankdistance*/)
        {
          //delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )
          digitalWrite(motorrelay,HIGH);
          motorstatus1=HIGH;
          
          Serial.println("Tank-1 Motor ON");
        }
        
        if(tank1manualremote==HIGH && (valve1==HIGH && valve_1_feedback==HIGH) && (powerstatus==HIGH && aclowvoltage==HIGH) && (pushonremote==HIGH || pushstate==HIGH) && tank1percent<=motoroffpercent_tank1_manual)
        {
          pushstate=HIGH;
          pushonremote=LOW;
          Serial.print("pushstate: ");
          Serial.println(pushstate);
          //delay(motordelay);                             //valve open first then motor on after 1 mnt ( 1 second = 1000 , 1 minuit = 60000 )
          digitalWrite(motorrelay,HIGH);
          motorstatus1=HIGH;
          
          Serial.println("Tank-1 Motor ON Manually");
        }
     
        if(tank1manualremote==LOW || tank1percent>=motoroffpercent_tank1_manual || valve_1_feedback==LOW || (powerstatus==LOW || aclowvoltage==LOW))
        {
          digitalWrite(motorrelay, LOW);
          motorstatus1=LOW;
          //valve1=LOW;
          //digitalWrite(valverelay1, valve1);
          pushstate=LOW;
          
          Serial.println("Tank-1 Motor OFF");
        } 
       }
    }

    
//////////////////////////////////////////////////////////////////////////// manual mode Finished ////////////////////////////////////////////////////////////////


  
}         //////////////////////////////////////////////////// Remote Blynk App Mode Finished //////////////////////////////////////////////////////








/////////////////////////////////////////////////////////////////////////// Arduino to ESP 01  Two Way Serial Communication /////////////////////////////


/////////////////////////////////////////////////////////////////// Arduino to ESP01 //////////////////////////////////////////


if(nano_esp01_serial.available() == 0 )
{
  sdata1 = tank1percent;    // tank 1 percent show
  sdata2 = tank2percent;    // tank 2 percent show
  sdata3 = valve_1_feedback;          // tank 1 valve open/close feedback Led Indication
  sdata4 = valve_2_feedback;          // tank 2 valve open/close feedback Led Indication
  sdata5 = motorstatus;           // motorrunningstatus() function required................
  sdata6 = tank1manualstatus;     // tank1manualswstatus() function required................
  sdata7 = tank2manualstatus;     // tank2manualswstatus() function required................
  sdata8 = localremotefeedback;   // see local/remote feedback function.....
  sdata9 = tank1swfeedback;       // see local/remote tank1 feedback function.....
  sdata10 = tank2swfeedback;      // see local/remote tank2 feedback function.....
  sdata11 = powerstatus;        // Power Fail feedback
  sdata12 = powerstatus;        // Power Fail LED Indication
  sdata13 = acvoltage;          // ac voltage
  sdata14 = aclowvoltage;          // ac low voltage
  sdata15 = aclowvoltagevalue;     // ac low voltage set value show
  sdata16 = motoronpercent;        // tank low level motor on set value show
  sdata17 = aclowvoltagebypass;    // ac low voltage bypass sw feedback
  
  
  cdata = cdata + sdata1 + "," + sdata2 + "," + sdata3 + "," + sdata4 + "," + sdata5 + "," + sdata6 + "," + sdata7 + "," + sdata8 + "," + sdata9 + "," + sdata10 + "," + sdata11 + "," + sdata12 + "," + sdata13 + "," + sdata14 + "," + sdata15 + "," + sdata16 + "," + sdata17; 
   
   
   Serial.println(cdata); 
   Serial.println("tank1,tank2,v/v1,v/v2,motorstatus,tank1manual,tank2manual,localremote,tank1sw,tank2sw,power,power,ac,aclow,aclowvalue,motoronpercent,aclowbypass");
   nano_esp01_serial.println(cdata);
   delay(1000); // 1000 milli seconds
   cdata = ""; 
}




motorrunningstatus();            // motor running status function for remote blynk app //
tank1manualswstatus();            // tank 1 manual function for remote blynk app //
tank2manualswstatus();            // tank 2 manual function for remote blynk app //




if(localremote==LOW)           ///////// Local Feedback //////
{
  localremotefeedbackswoff();
  //Serial.println("Local feedback");


  if(localremote==LOW && tank1manual==HIGH)     ////// Local Tank1 Feedback //////
  {
    tank1swfeedbackflag = 1;
    tank1swfeedbackon();
    //Serial.println("tank1 on feedback");
  }

  if(localremote==LOW && tank1manual==LOW)
  {
    tank1swfeedbackflag = 0;
    tank1swfeedbackoff();
    //Serial.println("tank1 off feedback");
  }
  

  if(localremote==LOW && tank2manual==HIGH)     ////// Local Tank2 Feedback //////
  {
    tank2swfeedbackflag = 1;
    tank2swfeedbackon();
    //Serial.println("tank2 on feedback");
  }

  if(localremote==LOW && tank2manual==LOW)
  {
    tank2swfeedbackflag = 0;
    tank2swfeedbackoff();
    //Serial.println("tank2 off feedback");
  }
}




if(localremote==HIGH)           ///////// Remote Feedback //////
{
  localremotefeedbackswon();
  

  if(localremote==HIGH && tank1manualremote==HIGH)     ////// Remote Tank1 Feedback //////
  {
    
    tank1swfeedbackon();
  }

  if(localremote==HIGH && tank1manualremote==LOW)
  {
    tank1swfeedbackoff();
  }
  

  if(localremote==HIGH && tank2manualremote==HIGH)     ////// Remote Tank2 Feedback //////
  {
    tank2swfeedbackon();
  }

  if(localremote==HIGH && tank2manualremote==LOW)
  {
    tank2swfeedbackoff();
  }
}


/////////////////////////////////////////////////////////////////// Arduino to ESP01 Finished //////////////////////////////////////////



/////////////////////////////////////////////////////////////////// ESP01 to Arduino //////////////////////////////////////////

if ( nano_esp01_serial.available() > 0 ) 
{
  data = nano_esp01_serial.parseInt();
  delay(100); 
  Serial.print("data received from ESP: ");
  Serial.println(data);
  
  /////////////////////////////////////////////
  
  if ( data == 201 || data == 201301)
  {
    localremote = LOW;            // Local Mode
  }
  
  if ( data == 301 || data == 301201  )
  {
    localremote = HIGH;            // Remote Active by Blynk App
  }
  
  /////////////////////////////////////////////
  
  if (( data == 10 ) && (localremote == HIGH))
  {
    tank1manualremote = LOW; 
  }
  
  
  if (( data == 11 ) && (localremote == HIGH))
  {
    tank1manualremote = HIGH;           // Tank 1 valve open by Blynk App
  }
  
  /////////////////////////////////////////////
  


  if (( data == 20 ) && (localremote == HIGH))
  {
    tank2manualremote = LOW; 
  }
  
  if (( data == 21 ) && (localremote == HIGH))
  {
    tank2manualremote = HIGH;           // Tank 2 valve open by Blynk App
  }
  
  /////////////////////////////////////////////
  


  if (localremote == HIGH)
  {
  if ( data == 30 )
  {
    pushonremote = LOW;
  }
  
  if (( data == 31 || data == 3130) && ((tank1manualstatus == HIGH && valve_1_feedback == HIGH)|| (tank2manualstatus == HIGH && valve_2_feedback == HIGH) ) )
  {
    pushonremote = HIGH;         // Motor Push ON by Blynk App
  }
  }
  /////////////////////////////////////////////


  if ( data >= 160 && data <= 220 )
  {
    aclowvoltagevalue = data;      // AC Low Voltage Value setting by Blynk App
  }
  /////////////////////////////////////////////


  if ( data == 50 || data == 60 || data == 70 )
  {
    motoronpercent = data;         // Motor ON Percent setting by Blynk App
  }
  /////////////////////////////////////////////
  

  if ( data == 2200 )
  {
    aclowvoltagebypass = LOW;        // AC Low Voltage Bypass Normal by Blynk App
  }
  
  if ( data == 2201 )
  {
    aclowvoltagebypass = HIGH;        // AC Low Voltage Bypass Bypassed by Blynk App
  }
  /////////////////////////////////////////////



  if ( data == 35 )
  {
    motoronalarm = LOW; 
  }
  
  if ( data == 36 )
  {
    motoronalarm = HIGH; 
  }
  /////////////////////////////////////////////
  

  
}


/////////////////////////////////////////////////////////////////// ESP01 to Arduino Finished //////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////// Arduino to ESP01  Two Way Serial Communication Finished /////////////////////////////




   
}               ////////////////////////////////////////////////////// void loop finished ///////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////// Feedback Function for Arduino to ESP01 /////////////////////////////////////////



void localremotefeedbackswoff()          /////////// Local/Remote Feedback OFF Function ///////////
{
  if(localremotefeedbackflag==0)
  {
    localremotefeedback = 201;
    localremotefeedbackflag = 1;
  }
}

void localremotefeedbackswon()          /////////// Local/Remote Feedback ON Function ///////////
{
  if(localremotefeedbackflag==1)
  {
    localremotefeedback = 301;
    localremotefeedbackflag = 0;
  }
}


void tank1swfeedbackoff()           ////////////// Local/Remote Tank1 Feedback OFF Function ////////////
{
  if(tank1swfeedbackflag == 0)
  {
    tank1swfeedback = 10;
    tank1swfeedbackflag = 1;
  }
}

void tank1swfeedbackon()           ////////////// Local/Remote Tank1 Feedback ON Function ////////////
{
  if(tank1swfeedbackflag == 1)
  {
    tank1swfeedback = 11;
    tank1swfeedbackflag = 0;
  }
}


void tank2swfeedbackoff()           ////////////// Local/Remote Tank2 Feedback OFF Function ////////////
{
  if(tank2swfeedbackflag == 0)
  {
    tank2swfeedback = 20;
    tank2swfeedbackflag = 1;
  }
}

void tank2swfeedbackon()           ////////////// Local/Remote Tank2 Feedback ON Function ////////////
{
  if(tank2swfeedbackflag == 1)
  {
    tank2swfeedback = 21;
    tank2swfeedbackflag = 0;
  }
}



void motorrunningstatus()              //////////////////////////// motor running status for remote blynk app ////////////////////////
{
  if ( motorstatus1==HIGH || motorstatus2==HIGH )
  {
    motorstatus=HIGH;
  }

  if ( motorstatus1==LOW && motorstatus2==LOW )
  {
    motorstatus=LOW;
  }
}

void tank1manualswstatus()              //////////////////////////// tank 1 manual status for remote blynk app ////////////////////////
{
  if(localremote==LOW)
  {
    tank1manualstatus = tank1manual;
  }

  if(localremote==HIGH)
  {
    tank1manualstatus = tank1manualremote;
  }
}


void tank2manualswstatus()              //////////////////////////// tank 2 manual status for remote blynk app ////////////////////////
{
  if(localremote==LOW)
  {
    tank2manualstatus = tank2manual;
  }

  if(localremote==HIGH)
  {
    tank2manualstatus = tank2manualremote;
  }
}


///////////////////////////////////////////////// Feedback Function for Arduino to ESP01 finished /////////////////////////////////////////





/*
void lowvoltbypassswfeedbackoff()           ////////////// Local/Remote Tank2 Feedback OFF Function ////////////
{
  if(lowvoltbypassswfeedbackflag == 0)
  {
    lowvoltbypassswfeedback = 0;
    lowvoltbypassswfeedbackflag = 1;
  }
}

void lowvoltbypassswfeedbackon()           ////////////// Local/Remote Tank2 Feedback ON Function ////////////
{
  if(lowvoltbypassswfeedbackflag == 1)
  {
    lowvoltbypassswfeedback = 21;
    lowvoltbypassswfeedbackflag = 0;
  }
}
*/
