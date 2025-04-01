#include "Global.h"

// Global variables
bool bluetoothConnexionState = false;
bool bluetoothRecievingData = false;
bool LedTaskRunning = true;
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

void taskLedStateAnimation(void *pvParameters)
{
  for (;;)
  {
    // Delete the task if data is recieved
    if (bluetoothRecievingData)
    {
#if DEBUG_CONNEXION
      if (xSemaphoreTake(serialMutex, portMAX_DELAY))
      { // 🔹 Attendre le mutex
        Serial.println("\n Data recieved. Deleting the task \n");
        Serial.flush();
        xSemaphoreGive(serialMutex); // 🔹 Libérer le mutex
      }
      else
      {
        Serial.println("\n Erreur : Impossible de prendre le mutex ! \n");
      }
#endif
      vTaskDelete(NULL);
    }
    if (LedTaskRunning)
    {
#if DEBUG_CONNEXION
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
#if DEBUG_CONNEXION
      if (xSemaphoreTake(serialMutex, portMAX_DELAY))
      { // 🔹 Attendre le mutex
        Serial.printf("\n Not doing animation. LedTaskRunning : %d   and state : %d \n", LedTaskRunning, a2dp_sink.get_connection_state());
        Serial.flush();
        xSemaphoreGive(serialMutex); // 🔹 Libérer le mutex
      }
      else
      {
        Serial.println("\n Erreur : Impossible de prendre le mutex ! \n");
      }
#endif
    }
  }
}

void createTaskLedAnimation(void)
{
#if DEBUG_CONNEXION
  if (serialMutex == NULL)
  {
    Serial.println("Erreur lors de la création du mutex");
  }
#endif

  BaseType_t taskStatus = xTaskCreatePinnedToCore(
      taskLedStateAnimation, // 🔹 Fonction exécutée
      "LEDTask",             // Nom de la tâche
      2048,                  // Taille de la stack
      NULL,                  // Paramètre
      1,                     // Priorité
      &ledTaskHandle,        // 🔹 Stocke le handle pour contrôle
      1                      // 🔹 Exécuter sur Core 1
  );

  // Print message indicating if task was created successfully
#if DEBUG_CONNEXION
  if (taskStatus != pdPASS)
  {
    Serial.println("Failed to create task");
    Serial.println(taskStatus); // Print error code
  }
  else
  {
    Serial.println("Task created successfully");
  }
#endif
}