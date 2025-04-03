#include "Global.h"

// Global variables
bool bluetoothConnexionState = false;
bool bluetoothRecievingData = false;
bool LedTaskRunning = true;
char taskName[] = "LED State Animation";
TaskHandle_t ledTaskHandle = NULL; // Stocke la tâche pour la contrôler
SemaphoreHandle_t serialMutex = xSemaphoreCreateMutex();

void runDisplay()
{
  FastLED.clear();
  int readSamples = readAudioSamples(); // Gets data from DMA buffers

  for (int i = 0; i < readSamples; i++) //
  {
    vReal[i] = readBuffer[i];
    vImag[i] = 0;
  }

  computeFFT();           // Applies all FFT calculations
  assignFreqBinsValues(); // Gets values for frequency bands
  processBandsRaw();      // Assign new band heights and peaks values
  drawBandsHeightsRaw();  // Draw new band heights values (0 = normal, 1 = fading)
  /*drawPeaks();            // Draw new band peaks   values
  EVERY_N_MILLISECONDS(PEAK_DECAY_SPEED)
  {
    peaksDecay();
  }*/
  FastLED.show();
}

void restartESP()
{
  Serial.println("Restarting ESP32...");
  delay(1000);
  ESP.restart();
}

void printWithMutexCheck(std::string message)
{
  if (xSemaphoreTake(serialMutex, portMAX_DELAY))
  { // 🔹 Wait for the mutex
    Serial.println(message.c_str());
    Serial.flush();
    xSemaphoreGive(serialMutex); // 🔹 Free the mutex
  }
  else
  {
    Serial.printf("\n Error ! Impossible to take the mutex !   At message : %s \n", message);
  }
}

void taskLedStateAnimation(void *pvParameters)
{
  for (;;) // Infinite loop (needed for the task to work properly)
  {
    // Suspend the task if data is recieved
    if (bluetoothRecievingData)
    {
#if DEBUG_THREADS
      char buffer[200];
      sprintf(buffer, "\n Data recieved. Suspending the task \n");
      printWithMutexCheck(buffer);
#endif
      vTaskSuspend(NULL); // Suspend task
    }
    if (LedTaskRunning && !bluetoothRecievingData)
    {
#if DEBUG_THREADS
      if (xSemaphoreTake(serialMutex, portMAX_DELAY))
      { // 🔹 Attendre le mutex
        Serial.println("\n Drawing LED State Animation (Task) \n");
        Serial.flush();
        xSemaphoreGive(serialMutex); // 🔹 Libérer le mutex
      }
      else
      {
        Serial.println("\n Erreur : Impossible de prendre le mutex ! \n");
      }
#endif
      drawConnectionState(a2dp_sink.get_connection_state());
    }
    else
    {
#if DEBUG_THREADS
      char buffer[200];
      sprintf(buffer, "\n Not doing animation. LedTaskRunning : %d   and state : %d \n", LedTaskRunning, a2dp_sink.get_connection_state());
      printWithMutexCheck(buffer);
#endif
    }
  }
}

void createTaskLedAnimation(void)
{
#if DEBUG_THREADS
  if (serialMutex == NULL)
  {
    Serial.println("Error while creating the mutex");
  }
#endif

  BaseType_t taskStatus = xTaskCreatePinnedToCore(
      taskLedStateAnimation, // Fonction exécutée
      "LEDTask",             // Nom de la tâche
      2048,                  // Taille de la stack
      NULL,                  // Paramètre
      1,                     // Priorité
      &ledTaskHandle,        // Stocke le handle pour contrôle
      1                      // Exécuter sur Core 1
  );

  // Print message indicating if task was created successfully
#if DEBUG_THREADS
  char buffer[200];
  if (taskStatus != pdPASS)
  {
    sprintf(buffer, "\n Failed to create task '%s'. Error code : %d \n", taskName, taskStatus);
  }
  else
  {
    sprintf(buffer, "\n Task '%s' created successfully \n", taskName);
  }
  printWithMutexCheck(buffer);
#endif
}