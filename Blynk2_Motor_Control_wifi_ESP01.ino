#define BLYNK_TEMPLATE_ID "name"
#define BLYNK_DEVICE_NAME "Water Pump Remote Control"
#define BLYNK_AUTH_TOKEN "token"

#include <ESP8266WiFi.h>


#include "SimpleTimer.h"
#define BLYNK_PRINT Serial



char auth[] = BLYNK_AUTH_TOKEN;



char ssid[] = "name";
char pass[] = "pass";


#include <BlynkSimpleEsp8266.h>


SimpleTimer timer;






String myString;
char rdata;
int rcv1val,rcv2val,rcv3val,rcv4val,rcv5val,rcv6val,rcv7val,rcv8val,rcv9val,rcv10val,rcv11val,rcv12val,rcv13val,rcv14val,rcv15val,rcv16val,rcv17val;        // received serial value
int sdata1,sdata2,sdata3,sdata4,sdata5,sdata6,sdata7,sdata8,sdata9,sdata10,sdata11,sdata12,sdata13,sdata14,sdata15,sdata16,sdata17;

int pinValue1;
int pinValue2;
int pinValue3;
int pinValue4;
int pinValue5;
int pinValue6;
int pinValue7;
int pinValue8;



// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.


void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, millis() / 1000);
  
}



void setup()
{
  
  Serial.begin(9600);


  Blynk.begin(auth, ssid, pass);
  
  
  timer.setInterval(1000L,receivevalue1); 
  timer.setInterval(1000L,receivevalue2);
  timer.setInterval(1000L,receivevalue3);
  timer.setInterval(1000L,receivevalue4); 
  timer.setInterval(1000L,receivevalue5);
  timer.setInterval(1000L,receivevalue6);
  timer.setInterval(1000L,receivevalue7);
  timer.setInterval(1000L,receivevalue8);
  timer.setInterval(1000L,receivevalue9);
  timer.setInterval(1000L,receivevalue10);
  timer.setInterval(1000L,receivevalue11);
  timer.setInterval(1000L,receivevalue12);
  timer.setInterval(1000L,receivevalue13);
  timer.setInterval(1000L,receivevalue14);
  timer.setInterval(1000L,receivevalue15);
  timer.setInterval(1000L,receivevalue16);
  timer.setInterval(1000L,receivevalue17);

}



void loop()
{
  if (Serial.available() == 0 )
  {
    Blynk.run();
    timer.run(); // Initiates BlynkTimer
  }
   
  if (Serial.available() > 0 )          ////////////////////////////// Arduino to ESP01 to Blynk App ////////////////////////////////
  {
    rdata = Serial.read(); 
    myString = myString+ rdata;
    //Serial.print(rdata);
    
    
    if( rdata == '\n')
    {
      //Serial.println(myString);

      
      String l = getValue(myString, ',', 0);
      String m = getValue(myString, ',', 1);
      String n = getValue(myString, ',', 2);
      String o = getValue(myString, ',', 3);
      String p = getValue(myString, ',', 4);
      String q = getValue(myString, ',', 5);
      String r = getValue(myString, ',', 6);
      String s = getValue(myString, ',', 7);
      String t = getValue(myString, ',', 8);
      String u = getValue(myString, ',', 9);
      String v = getValue(myString, ',', 10);
      String w = getValue(myString, ',', 11);
      String x = getValue(myString, ',', 12);
      String y = getValue(myString, ',', 13);
      String z = getValue(myString, ',', 14);
      String z1 = getValue(myString, ',', 15);
      String z2 = getValue(myString, ',', 16);
      
      
      rcv1val = l.toInt();
      rcv2val = m.toInt();
      rcv3val = n.toInt();
      rcv4val = o.toInt();
      rcv5val = p.toInt();
      rcv6val = q.toInt();
      rcv7val = r.toInt();
      rcv8val = s.toInt();
      rcv9val = t.toInt();
      rcv10val = u.toInt();
      rcv11val = v.toInt();
      rcv12val = w.toInt();
      rcv13val = x.toInt();
      rcv14val = y.toInt();
      rcv15val = z.toInt();
      rcv16val = z1.toInt();
      rcv17val = z2.toInt();
      
      
      
      myString = "";
      
      
      receivevalue1();
      receivevalue2();
      receivevalue3();
      receivevalue4();
      receivevalue5();
      receivevalue6();
      receivevalue7();
      receivevalue8();
      receivevalue9();
      receivevalue10();
      receivevalue11();
      receivevalue12();
      receivevalue13();
      receivevalue14();
      receivevalue15();
      receivevalue16();
      receivevalue17();

    }
  }
}



void receivevalue1()
{
  int sdata1 = rcv1val;
  //int sdata1 = 60;  
  Blynk.virtualWrite(V2, sdata1);       // tank 1 percent
}

void receivevalue2()
{
  int sdata2 = rcv2val;
  //int sdata2 = 40;
  Blynk.virtualWrite(V3, sdata2);       // tank 2 percent
}

void receivevalue3()
{
  if(rcv3val==1)
  {
    sdata3 = 255;
  }
  if(rcv3val==0)
  {
    sdata3 = 0;
  }
  Blynk.virtualWrite(V11, sdata3);       // tank 1 valve LED Indication
}

void receivevalue4()
{
  if(rcv4val==1)
  {
    sdata4 = 255;
  }
  if(rcv4val==0)
  {
    sdata4 = 0;
  }
  Blynk.virtualWrite(V21, sdata4);       // tank 2 valve LED Indication
}

void receivevalue5()
{
  if(rcv5val==1)
  {
    sdata5 = 255;
    //Blynk.notify("Motor Running");
  }
  if(rcv5val==0)
  {
    sdata5 = 0;
    //Blynk.notify("Motor OFF");
  }
  Blynk.virtualWrite(V31, sdata5);       // motor running LED Indication
}

void receivevalue6()
{
  if(rcv6val==1)
  {
    sdata6 = 0;
    //Blynk.notify("Tank 1 Auto");
  }
  if(rcv6val==0)
  {
    sdata6 = 255;
    //Blynk.notify("Tank 1 OFF");
  }
  Blynk.virtualWrite(V12, sdata6);            // tank 1 OFF LED Indication
  Blynk.setProperty(V12, "color", "#D3435C");
}

void receivevalue7()
{
  if(rcv7val==1)
  {
    sdata7 = 0;
    //Blynk.notify("Tank 2 Auto");
  }
  if(rcv7val==0)
  {
    sdata7 = 255;
    //Blynk.notify("Tank 2 OFF");
  }
  Blynk.virtualWrite(V22, sdata7);            // tank 2 OFF LED Indication
  Blynk.setProperty(V22, "color", "#D3435C");
}

void receivevalue8()
{
  if(rcv8val==201)
  {
    sdata8 = rcv8val;
    Blynk.virtualWrite(V1, sdata8);             // local remote sw feedback
    Blynk.setProperty(V1,"offLabel","Local");
    Blynk.setProperty(V1,"color","#8C33B6");
  }
  if(rcv8val==301)
  {
    sdata8 = rcv8val;
    Blynk.virtualWrite(V1, sdata8);            // local remote sw feedback
    Blynk.setProperty(V1,"onLabel","Remote");
    Blynk.setProperty(V1,"color","#04C0F8");
  }
}

void receivevalue9()
{
  if(rcv9val==10)
  {
    sdata9 = rcv9val;
    Blynk.virtualWrite(V10, sdata9);                 // tank1 sw feedback
    Blynk.setProperty(V10,"offLabel","Tank 1 OFF");
    Blynk.setProperty(V10,"color","#D3435C");
  }
  if(rcv9val==11)
  {
    sdata9 = rcv9val;
    Blynk.virtualWrite(V10, sdata9);                // tank1 sw feedback
    Blynk.setProperty(V10,"onLabel","Tank 1 AUTO");
    Blynk.setProperty(V10,"color","#23C48E");
  }
}

void receivevalue10()
{
  if(rcv10val==20)
  {
    sdata10 = rcv10val;
    Blynk.virtualWrite(V20, sdata10);                 // tank2 sw feedback
    Blynk.setProperty(V20,"offLabel","Tank 2 OFF");
    Blynk.setProperty(V20,"color","#D3435C");
  }
  if(rcv10val==21)
  {
    sdata10 = rcv10val;
    Blynk.virtualWrite(V20, sdata10);                 // tank2 sw feedback
    Blynk.setProperty(V20,"onLabel","Tank 2 AUTO");
    Blynk.setProperty(V20,"color","#23C48E");
  }
}

void receivevalue11()
{
  if(rcv11val==0)
  {
    sdata11 = 201;
    Blynk.virtualWrite(V1, sdata11);                     // Power Fail feedback
    Blynk.setProperty(V1,"offLabel","Local Power Fail");
    Blynk.setProperty(V1,"color","#D3435C");
  }
}

void receivevalue12()
{
  if(rcv12val==1)
  {
    sdata12 = 0;
  }
  if(rcv12val==0)
  {
    sdata12 = 255;
    //Blynk.notify("Home West Side / Motor Power Fail");
  }

  if(rcv12val==1 && rcv14val==0)   // Low Volt Power Fail
  {
    sdata12 = 255;
  }
  Blynk.virtualWrite(V40, sdata12);            // Power Fail LED Indication
  Blynk.setProperty(V40, "color", "#D3435C");
}

void receivevalue13()
{
  int sdata13 = rcv13val;
  Blynk.virtualWrite(V41, sdata13);            // ac voltage show
}

void receivevalue14()
{
  if(rcv14val==1)
  {
    sdata14 = 0;
  }
  if(rcv14val==0)
  {
    sdata14 = 255;
  }
  Blynk.virtualWrite(V42, sdata14);            // ac low voltage led indication
  Blynk.setProperty(V42, "color", "#D3435C");
}


void receivevalue15()
{
  int sdata15 = rcv15val;
  Blynk.virtualWrite(V46, sdata15);            // ac low voltage set value
}


void receivevalue16()
{
  int sdata16 = rcv16val;
  Blynk.virtualWrite(V36, sdata16);            // tank low level set value
}


void receivevalue17()
{
  if(rcv17val==0)
  {
    sdata17 = 2200;
    Blynk.virtualWrite(V48, sdata17);                 // ac low voltage bypass sw feedback
    Blynk.setProperty(V48,"offLabel","Normal");
    Blynk.setProperty(V48,"color","#D3435C");
  }
  if(rcv17val==1)
  {
    sdata17 = 2201;
    Blynk.virtualWrite(V48, sdata17);                 // ac low voltage bypass sw feedback
    Blynk.setProperty(V48,"onLabel","Low Voltage Bypass");
    Blynk.setProperty(V48,"color","#23C48E");
  }
}





String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}






///////////////////////////////////////////////////////// Blynk App to ESP01 to Arduino ////////////////////////////////////////

BLYNK_WRITE(V1)    //////////////////// Local Remote //////////////////
{
   pinValue1 = param.asInt();
   Serial.print(pinValue1);
}



BLYNK_WRITE(V10)     //////////////////// Tank 1 auto manual /////////////////////
{
   pinValue2 = param.asInt();
   Serial.print(pinValue2);
}


BLYNK_WRITE(V20)     //////////////////// Tank 2 auto manual /////////////////////
{
   pinValue3 = param.asInt();
   Serial.print(pinValue3);
}


BLYNK_WRITE(V30)     //////////////////// Motor Push ON /////////////////////
{
   pinValue4 = param.asInt();
   Serial.print(pinValue4);
}


BLYNK_WRITE(V45)     //////////////////// ac low voltage value /////////////////////
{
   pinValue5 = param.asInt();
   Serial.print(pinValue5);
}


BLYNK_WRITE(V35)     //////////////////// motor on percent /////////////////////
{
   pinValue6 = param.asInt();
   Serial.print(pinValue6);
}


BLYNK_WRITE(V48)     //////////////////// AC Low Voltage Bypass /////////////////////
{
   pinValue7 = param.asInt();
   Serial.print(pinValue7);
}


BLYNK_WRITE(V38)     //////////////////// motor on alarm /////////////////////
{
   pinValue8 = param.asInt();
   Serial.print(pinValue8);
}
