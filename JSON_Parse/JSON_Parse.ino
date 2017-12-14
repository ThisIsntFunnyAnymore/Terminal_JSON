// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2017
// MIT License

#include <ArduinoJson.h>

void setup() {
  // Initialize serial port
  Serial.begin(9600);
  while (!Serial) continue;

  // Memory pool for JSON object tree.
  //
  // Inside the brackets, 200 is the size of the pool in bytes.
  // Don't forget to change this value to match your JSON document.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<200> jsonBuffer;

  // StaticJsonBuffer allocates memory on the stack, it can be
  // replaced by DynamicJsonBuffer which allocates in the heap.
  //
  // DynamicJsonBuffer  jsonBuffer(200);

  // JSON input string.
  //
  // It's better to use a char[] as shown here.
  // If you use a const char* or a String, ArduinoJson will
  // have to make a copy of the input in the JsonBuffer.
  char json[] =
      "{\"hostname\":\"myhost.noip.com\",\"SSID\":\"TheOtherNetwork_2G\","
      "\"auth\":\"ThreeCatHouse\",\"token\":\"f2435662623040aaba317a4e503594b7\"}";

  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.parseObject(json);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
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
}

void loop() {
  // not used in this example
}

