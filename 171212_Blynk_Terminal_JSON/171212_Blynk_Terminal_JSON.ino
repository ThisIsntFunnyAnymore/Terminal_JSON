/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  =>
  =>          USB HOWTO: http://tiny.cc/BlynkUSB
  =>

  You can send/receive any data using WidgetTerminal object.

  App project setup:
    Terminal widget attached to Virtual Pin V1
 *************************************************************/

/* Comment this out to disable prints and save space */
//#define BLYNK_PRINT SwSerial

// Arduino JSON Library
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(10, 11); // RX, TX

#include <BlynkSimpleStream.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "f***********************7";

// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);

//initialize the json string. at some point this will pull from EEPROM
String json;

// Create a JSON buffer; parse json variable, add JSONKey = JSONVal, write the
// JSON string back to json
void addJSON(char JSONKey[10], char JSONVal[40]) {
  /*debug 2017/12/13 we know the values are being passed in correctly
    terminal.print("addJSON outputs");
    terminal.print("\t");
    terminal.print(JSONKey);
    terminal.print("\t");
    terminal.print(JSONVal);
    terminal.print("\t");
    // end debug */

  // Memory pool for JSON object tree. Checked with assistant on
  // arduinojson.org. This may not be scoped correctly and may need
  // to be moved ahead of the void setup() function
  StaticJsonBuffer<200> jsonBuffer;
  // attempt to parse the root object of the tree
  JsonObject& root = jsonBuffer.parseObject(json);
  // Test if parsing succedes
  if (!root.success()) {
    // if failure, create json
    terminal.println(F("addJSON: Creating new JSON object"));
    JsonObject& root = jsonBuffer.createObject();
  } else {
    terminal.println(F("addJSON: Using existing JSON object string"));
    //  root.prettyPrintTo(terminal);
    //  terminal.flush();
  }
  root[JSONKey] = JSONVal;
  // I think I need to initialize the json character array to "null it" otherwise it
  // will concatenate? - 2017/12/14 yes this is true
  json = "";
  root.printTo(json);
  //did I break the json string?
  //Originally debug code, but I think we should keep it.
  if (root.success()) {
    terminal.println(F("addJSON: Success"));
    //terminal.println(root.success());
    root.prettyPrintTo(terminal);
    terminal.print("\n");
    //  terminal.flush();
  } else {
    String failOne = "BLYNK_WRITE(V1): Failed to set ";
    String failTwo = " value ";
    String failMsg = failOne + JSONKey + failTwo + JSONVal;
    terminal.println(failMsg);
    //terminal.println("Failed to set " + String(par) " value " + String(parVal));
  }
  // Ensure everything is sent
  terminal.flush();
  //  return root.success();
  //the root memory buffer should clear when function exits.
}

// Move the error checking to a subroutine. The substring command
// won't work the way I want it to
int checkPar(char p[10]) {
  int parSuccess = 0;
  terminal.println(F("checking parameter..."));  //debugging; comment out final
  char *a_pars[6] = {"hostname", "SSID", "auth", "token", "print", "done"};
  for (int i = 0; i < 6; i++) {
    if (strcmp(a_pars[i], p) == 0) {
      parSuccess = 1;
      break;
    }
  }
  return parSuccess;
}

// Print the output of the JSON object
void printJSON() {
  // Debug
  terminal.println(F("printJSON: Printing JSON..."));
  // Memory pool for JSON object tree. Checked with assistant on
  // arduinojson.org. This may not be scoped correctly and may need
  // to be moved ahead of the void setup() function
  StaticJsonBuffer<200> jsonBuffer;
  // Create the root object of the tree
  JsonObject& root = jsonBuffer.parseObject(json);
  if (root.success()) {
    root.prettyPrintTo(terminal);
  } else {
    terminal.println(F("printJSON: Failed to print (no JSON object to print?)"));
  }
  /* DEPRECATED prettyPrintTo does work with terminal. Remove on cleanup
      if prettyprint doesn't work with terminal use this code
      const char* hostname = root["hostname"];
      const char* mySSID = root["SSID"];
      const char* auth = root["auth"];
      const char* token = root["token"];

      // Print values.
      Serial.print("Server Host: ");
      Serial.println(hostname);
      Serial.print("WiFi SSID: ");
      Serial.println(mySSID);
      Serial.print("Wifi Pwd: ");
      Serial.println(auth);
      Serial.print("Blynk Token: ");
      Serial.println(token);
    ========================================*/
  terminal.flush();
}

// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
BLYNK_WRITE(V1)
{
  //get the parameter written to V1 as string
  //char * tokens[2] = getTokens(param.asStr());
  String sentence = param.asStr();
  char par[10];
  char parVal[40];
  /*2017/12/14 we know we are passing sentence correctly
    //Debugging code. Do bulk comment for final
    terminal.print("\n");
    terminal.print("So far so good. We have the string \"");
    terminal.print(sentence);
    terminal.println("\"");
    //end debugging code*/

  sscanf(sentence.c_str(), "%s %s", par, parVal);

  /*2017/12/14 we know we can parse sentence correctly, although may need to
     figure out how to trim parVal to remove trailing spaces often left by
     mobile device keyboards.
    //Debugging code
    terminal.print("Now we have parsed sentence into \"");
    terminal.print(par);
    terminal.print("\" and \"");
    terminal.print(parVal);
    terminal.println("\"");
    terminal.print("and checkPar = ");
    terminal.println(checkPar(par));
    //End debugging code*/

  if (checkPar(par) != 0) {
    if (strcmp(par, "print") == 0) {
      printJSON();
      // we are now checking for parameters before entering this if() structure
      // so no need to error check par
    } else if (strcmp(par, "done") == 0) {

      //writeEEPROM();

    } else  {
      //made addJSON a void function and handle failure reporting there
      addJSON(par, parVal);
    }
  } else {
    String failOne = "BLYNK_WRITE(V1)";
    String failTwo = " is not a valid parameter command.";
    String failMsg = failOne + par + failTwo;
    terminal.println(failMsg);
  }
}

// 2017/12/14 Original terminal code. Delete in next commit
// if you type "Marco" into Terminal Widget - it will respond: "Polo:"
//  if (String("Marco") == param.asStr()) {
//    terminal.println("You said: 'Marco'") ;
//    terminal.println("I said: 'Polo'") ;
//  } else {
//
//    // Send it back
//    terminal.print("You said:");
//    terminal.write(param.getBuffer(), param.getLength());
//    terminal.println();
//  }


void setup()
{
  // Debug console
  SwSerial.begin(9600);

  // Blynk will work through Serial
  // Do not read or write this serial manually in your sketch
  Serial.begin(9600);


  Blynk.begin(Serial, auth);


  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Enter data as follows"));
  terminal.println(F("to set up board for local server"));
  // set hostname or default to blynk-cloud
  terminal.println(F("type:parameter [YourValueHere]"));
  terminal.println(F("Available parameter values: hostname, SSID, auth,"));
  terminal.println(F("token, print, done. print has no value. Type 'done'"));
  terminal.println(F("to write the JSON string to EEPROM"));
  terminal.flush();

  //Initialize with default hostname. This means that if I start with
  // a print command I should get a json string back
  // as of 2017/12/14 this is not working. I may need to add a blocking function
  // to prevent this from running until blynk is connected.
  //addJSON("hostname", "blynk-cloud.com");
}

void loop()
{
  Blynk.run();
}







