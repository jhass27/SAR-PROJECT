#include <Process.h>

void setup() {
  // Initialize Bridge
  Bridge.begin();
  // Initialize Serial
  Serial.begin(115200);
  // Wait until a Serial Monitor is connected.
  //while (!Serial);
}

void loop() {
  // Do nothing here.
  Process p;    // Create a process and call it "p"
  p.begin("/home/sar/Documents/VNA_scripts/scripts/test5_ohneAmplifier/main_1.py"); // Process that launch the "do_p.py" command
//  p.addParameter("500"); // Add the parameter to "Temp"
//  p.addParameter("1000"); // Add the parameter to "Wilg"
  p.run();    // Run the process and wait for its termination

  // Print arduino logo over the Serial
  // A process output can be read with the stream methods
  while (p.available() > 0) {
    char c = p.read();
    Serial.print(c);
  }
  // Ensure the last bit of data is sent.
  Serial.flush();
  delay(10000);
}
