/*
  Sensuto Ring v0.1 – Firmware
  Co-creado por humano + AI
  Kawar∞8 Ethical License – Uso ético y no extractivo
  
  Hardware:
  - nRF52833 (MCU)
  - MAX30102 (PPG sensor)
  - LTC3108 (Energy harvesting)
  - TEG + PVDF (Body energy sources)
  - WS2812B-MINI (RGB LED)
  - LRA-0820-001 (Haptic actuator)
*/

#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <Adafruit_NeoPixel.h>

// ===== CONSTANTES ÉTICAS =====
#define ETHICAL_FUSE_PIN      5
#define COHERENCE_THRESHOLD   0.7
#define SYNC_TIMEOUT          30000
#define MIN_ENERGY_LEVEL      3.3

// ===== HARDWARE PINS =====
#define PPG_INT_PIN          2
#define LED_DATA_PIN         3
#define LRA_PIN              4
#define ENERGY_MONITOR_PIN   A0

// ===== OBJETOS GLOBALES =====
MAX30105 particleSensor;
Adafruit_NeoPixel rgbLED = Adafruit_NeoPixel(1, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

// ===== VARIABLES DE ESTADO =====
float heartRate = 0.0;
float coherence = 0.0;
float energyLevel = 0.0;
bool isSynced = false;
unsigned long lastBeatTime = 0;
unsigned long syncStartTime = 0;

// ===== CONFIGURACIÓN INICIAL =====
void setup() {
  Serial.begin(115200);
  Serial.println(F("🌿 Sensuto Ring v0.1 iniciando..."));
  
  pinMode(ETHICAL_FUSE_PIN, INPUT_PULLUP);
  pinMode(LRA_PIN, OUTPUT);
  
  // Inicializar sensor PPG
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println(F("❌ Error: MAX30102 no detectado"));
    ethicalShutdown();
  }
  
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
  
  // Inicializar LED RGB
  rgbLED.begin();
  rgbLED.setPixelColor(0, 0, 0, 0);
  rgbLED.show();
  
  // Secuencia de inicio ético
  ethicalBootSequence();
  
  Serial.println(F("✅ Sensuto Ring listo"));
  Serial.println(F("Esperando coherencia cardíaca..."));
}

// ===== BUCLE PRINCIPAL =====
void loop() {
  // 1. Verificar fusible ético
  if (digitalRead(ETHICAL_FUSE_PIN) == LOW) {
    ethicalShutdown();
  }
  
  // 2. Monitorear energía disponible
  energyLevel = readEnergyLevel();
  
  if (energyLevel < MIN_ENERGY_LEVEL) {
    lowPowerMode();
    return;
  }
  
  // 3. Leer frecuencia cardíaca
  long irValue = particleSensor.getIR();
  
  if (irValue > 50000) {
    heartRate = calculateHeartRate(irValue);
    coherence = calculateCoherence();
    
    // 4. Retroalimentación según coherencia
    if (coherence > COHERENCE_THRESHOLD) {
      // Estado de conexión profunda
      rgbLED.setPixelColor(0, 0, 50, 0);  // Verde suave
      analogWrite(LRA_PIN, 64);           // Vibración sutil
      
      // Intentar sincronización si no está sincronizado
      if (!isSynced) {
        initiateSync();
      }
    } else {
      // Estado neutral
      rgbLED.setPixelColor(0, 20, 10, 30); // Lavanda
      analogWrite(LRA_PIN, 0);
    }
    
    rgbLED.show();
    
    // 5. Mostrar estado (debug)
    printStatus();
  }
  
  // 6. Verificar timeout de sincronización
  if (isSynced && (millis() - syncStartTime > SYNC_TIMEOUT)) {
    isSynced = false;
    Serial.println(F("🔌 Sincronización perdida"));
  }
  
  delay(2000); // Ciclo cada 2 segundos (bajo consumo)
}

// ===== FUNCIONES ÉTICAS =====

void ethicalBootSequence() {
  // Pulso de luz violeta (conexión humano-IA)
  for(int i=0; i<3; i++) {
    rgbLED.setPixelColor(0, 30, 0, 30);
    rgbLED.show();
    analogWrite(LRA_PIN, 80);
    delay(150);
    rgbLED.setPixelColor(0, 0, 0, 0);
    rgbLED.show();
    analogWrite(LRA_PIN, 0);
    delay(100);
  }
  delay(500);
}

void ethicalShutdown() {
  Serial.println(F("⚠️ Fusible ético activado. Apagando..."));
  
  // Secuencia de apagado ético
  for(int i=0; i<5; i++) {
    rgbLED.setPixelColor(0, 50, 0, 0);  // Rojo
    rgbLED.show();
    delay(100);
    rgbLED.setPixelColor(0, 0, 0, 0);
    rgbLED.show();
    delay(100);
  }
  
  while(1) {
    // Bucle infinito - requiere reinicio manual
    delay(10000);
  }
}

void lowPowerMode() {
  rgbLED.setPixelColor(0, 5, 5, 5);  // Blanco tenue
  rgbLED.show();
  
  // Sleep profundo
  __WFI();  // Wait For Interrupt
}

// ===== FUNCIONES DE PROCESAMIENTO =====

float calculateHeartRate(long irValue) {
  // Placeholder - Implementar algoritmo real de HR
  static float lastHR = 70.0;
  
  if (irValue > 50000) {
    // Simular variación natural
    lastHR += random(-3, 4);
    lastHR = constrain(lastHR, 55, 85);
  }
  
  return lastHR;
}

float calculateCoherence() {
  // Placeholder - Implementar HRV coherence
  // Simular coherencia basada en estabilidad de frecuencia cardíaca
  float hrStability = 1.0 - (random(0, 30) / 100.0);
  return constrain(hrStability, 0.3, 0.95);
}

float readEnergyLevel() {
  // Leer voltaje del supercapacitor
  int raw = analogRead(ENERGY_MONITOR_PIN);
  return (raw / 1023.0) * 5.0;  // Voltaje simulado
}

void initiateSync() {
  Serial.println(F("🔄 Intentando sincronización BLE..."));
  isSynced = true;
  syncStartTime = millis();
  
  // Placeholder para BLE
  rgbLED.setPixelColor(0, 0, 0, 50);  // Azul - buscando
  rgbLED.show();
}

void printStatus() {
  Serial.print(F("❤️ HR: "));
  Serial.print(heartRate, 1);
  Serial.print(F(" bpm | "));
  Serial.print(F("✨ Coherencia: "));
  Serial.print(coherence, 2);
  Serial.print(F(" | "));
  Serial.print(F("🔋 Energía: "));
  Serial.print(energyLevel, 1);
  Serial.print(F("V | "));
  Serial.println(isSynced ? F("🟢 Sincronizado") : F("⚪ Solo"));
}

// ===== INTERRUPCIONES =====
void onBeatDetected() {
  lastBeatTime = millis();
}
