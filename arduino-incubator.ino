/**
 * Incubator Watcher
 *
 * Used to watch if my (cheap) egg incubator is working fine
 *
 * @author Rizwan <m.rizwan_47@yahoo.com>
 *
 * Originally learnt from: https://learn.adafruit.com/dht/overview
 */

#include  <Adafruit_Sensor.h>
#include  <DHT.h>
#include  <DHT_U.h>

#define HIGH_TEMP_WARNING_PIN	13     // High Temperature Warning Pin
#define LOW_TEMP_WARNING_PIN	6      // Low Temperature Warning Pin

#define HIGH_HUM_WARNING_PIN	4      // High Humidity Warning Pin
#define LOW_HUM_WARNING_PIN		5      // Low Humidity Warning Pin

#define DHT_SENSOR_PIN				7      // Pin to collect sensor data
#define DHT_SENSOR_TYPE				DHT22  // Sensor type

#define FAN_TRIGGER_PIN				XX     // Pin to collect sensor data
#define HEATER_TRIGGET_PIN		XX     // Sensor type

// Configurable Settings
#define MIN_TEMPRATURE				37.5    // Minimum temprature
#define MAX_TEMPRATURE				38.8    // Maximum temprature
#define MIN_HUMIDITY					45      // Minimum temprature
#define MAX_HUMIDITY					70      // Maximum temprature

DHT_Unified dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);
uint32_t delayMS;

void setup(){

  Serial.begin(9600);
  dht.begin();

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  pinMode(HIGH_TEMP_WARNING_PIN, OUTPUT);
  pinMode(LOW_TEMP_WARNING_PIN, OUTPUT);
  pinMode(HIGH_HUM_WARNING_PIN, OUTPUT);
  pinMode(LOW_HUM_WARNING_PIN, OUTPUT);

}

void loop() {

  sensors_event_t event;

  dht.temperature().getEvent(&event);

  if(isnan(event.temperature)){
    Serial.println("Error reading temperature!");
  }else{

    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" C");

    if( event.temperature < MIN_TEMPRATURE ){
      digitalWrite(HIGH_TEMP_WARNING_PIN, LOW);
      digitalWrite(LOW_TEMP_WARNING_PIN, HIGH);
      Serial.println("WARNING: Low Temprature");
    }else if( event.temperature > MAX_TEMPRATURE ){
      digitalWrite(HIGH_TEMP_WARNING_PIN, HIGH);
      digitalWrite(LOW_TEMP_WARNING_PIN, LOW);
      Serial.println("WARNING: High Temprature");
    }else{
      digitalWrite(HIGH_TEMP_WARNING_PIN, LOW);
      digitalWrite(LOW_TEMP_WARNING_PIN, LOW);
    }

  }

  dht.humidity().getEvent(&event);

  if(isnan(event.relative_humidity)){
    Serial.println("Error reading humidity!");
  }else{

    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");

    if( event.temperature < MIN_HUMIDITY ){
      digitalWrite(HIGH_HUM_WARNING_PIN, LOW);
      digitalWrite(LOW_HUM_WARNING_PIN, HIGH);
      Serial.println("WARNING: Low Humidity");
    }else if( event.temperature > MAX_HUMIDITY ){
      digitalWrite(HIGH_HUM_WARNING_PIN, HIGH);
      digitalWrite(LOW_HUM_WARNING_PIN, LOW);
      Serial.println("WARNING: High Humidity");
    }else{
      digitalWrite(HIGH_HUM_WARNING_PIN, LOW);
      digitalWrite(LOW_HUM_WARNING_PIN, LOW);
    }

  }

  Serial.println("-----");
  delay(delayMS);

}
