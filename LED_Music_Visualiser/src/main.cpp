#include "Global.h"

/***** SETUP *****/
void setup()
{
#if DEBUG
  Serial.begin(115200);
  Serial.println("***** Starting setup *****");
#endif
  setupI2sAudio();
  initDisplay();
  initVariables();

#if DEBUG
  Serial.println("***** Setup finished *****");
  delay(1000);
#endif
}

/***** MAIN LOOP *****/
void loop()
{
  // In bluetooth mode, only display if connected
  if ((AUDIO_MODE == MODE_BLUETOOTH) && bluetoothConnexionState)
  {
    runDisplay();
  }
}
