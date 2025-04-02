#include "Global.h"

/***** SETUP *****/
void setup()
{
#if DEBUG || DEBUG_BT || DEBUG_DATA || DEBUG_CONNEXION || DEBUG_THREADS
  Serial.begin(115200);
  Serial.println("***** Starting setup *****");
#endif

  createTaskLedAnimation();

  initDisplay();
  initVariables();
  setupI2sAudio();

#if DEBUG || DEBUG_BT || DEBUG_DATA
  Serial.println("***** Setup finished *****");
#endif
}

/***** MAIN LOOP *****/
void loop()
{
  if (AUDIO_MODE == MODE_BLUETOOTH)
  {
    // checkBTConnectionState();
    //   Run main display in BT Mode only if recieving data and connected
    if (bluetoothConnexionState && bluetoothRecievingData)
    {
      runDisplay();
    }
  }
}
