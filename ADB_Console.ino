#include <SPI.h>
#include <Adb.h>

// Adb connection.
Connection * connection;

char ConsoleString[1024];         // a string to hold incoming console data
int ConsoleIndex=0;
boolean ConsoleComplete = false;   // This will be set to true once we have a full string


// Event handler for the shell connection. 
void adbEventHandler(Connection * connection, adb_eventType event, uint16_t length, uint8_t * data)
{
  int i;

  if (event == ADB_CONNECTION_RECEIVE) {
    for (i=0; i<length; i++) {
      //Serial.print(data[i]);
      Serial.write(data[i]);
    }
  }
  
  if (event == ADB_CONNECTION_FAILED) {
    Serial.println("ADB_CONNECTION_FAILED");
  }

  if (event == ADB_CONNECTION_OPEN) {
    Serial.println("ADB_CONNECTION_OPEN");
  }

  if (event == ADB_CONNECTION_CLOSE) {
    Serial.println("ADB_CONNECTION_CLOSE");
  }


  if (event == ADB_CONNECT) {
    Serial.println("ADB_CONNECT");
  }

  if (event == ADB_DISCONNECT) {
    Serial.println("ADB_DISCONNECT");
  }

}

void setup()
{

  // Initialise serial port
  Serial.begin(115200);

  // Initialise the ADB subsystem.  
  ADB::init();

  // Open an ADB stream to the phone's shell. Auto-reconnect
  //connection = ADB::addConnection("shell:exec logcat", true, adbEventHandler);
  //connection = ADB::addConnection("shell:exec logcat -s Cyclops:D", true, adbEventHandler);
  //connection = ADB::addConnection("shell:exec sh", true, adbEventHandler);  
  connection = ADB::addConnection("tcp:6000", true, adbEventHandler);  


}

void loop()
{
  
   if (ConsoleComplete) {
    connection->write(ConsoleIndex, (uint8_t*)ConsoleString);
    ConsoleComplete = false;
    ConsoleIndex=0;
   }
  
  // Poll the ADB subsystem.
  ADB::poll();
}

void serialEvent(){ //Interrupt when the Console says something
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to ConsoleString:
    ConsoleString[ConsoleIndex] = inChar;
    ConsoleIndex++;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      ConsoleComplete = true;
    } 
  }
}

