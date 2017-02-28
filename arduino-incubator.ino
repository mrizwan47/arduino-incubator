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

#define HIGH_TEMP_WARNING_PIN	13		// High Temperature Warning Pin
#define LOW_TEMP_WARNING_PIN	6			// Low Temperature Warning Pin

#define HIGH_HUM_WARNING_PIN	4			// High Humidity Warning Pin
#define LOW_HUM_WARNING_PIN		5			// Low Humidity Warning Pin

#define DHT_SENSOR_PIN				8			// Pin to collect sensor data
#define DHT_SENSOR_TYPE				DHT22	// Sensor type

#define FAN_TRIGGER_PIN				9			// Pin to trigger Fan
#define HEATER_TRIGGET_PIN		10		// Pin to trigger heater/bulb

#define AUTO_FAN							true	// Enable automatically switching of fan
#define AUTO_HEAT							true	// Enable automatically switching of heat

// Configurable Settings
#define MIN_TEMPRATURE				32		// Minimum temprature
#define MAX_TEMPRATURE				33		// Maximum temprature
#define MIN_HUMIDITY					45		// Minimum Humidity
#define MAX_HUMIDITY					70		// Maximum Humidity

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

	pinMode(FAN_TRIGGER_PIN, OUTPUT);
	pinMode(HEATER_TRIGGET_PIN, OUTPUT);

}

void switch_fan( bool on ) {

	if( on ){
		digitalWrite(FAN_TRIGGER_PIN, HIGH);
	}else{
		digitalWrite(FAN_TRIGGER_PIN, LOW);
	}

}

void switch_heat( bool on ) {

	if( on ){
		digitalWrite(HEATER_TRIGGET_PIN, HIGH);
	}else{
		digitalWrite(HEATER_TRIGGET_PIN, LOW);
	}

}

void loop() {

	if( Serial.available() ){

		char com = Serial.read();

		if( com == 'FanOff' ){
			switch_fan(false);
			AUTO_FAN	=	false;
		}

		if( com == 'FanOn' ){
			switch_fan(true);
			AUTO_FAN	=	false;
		}

		if( com == 'HeatOff' ){
			switch_heat(false);
			AUTO_HEAT	=	false;
		}

		if( com == 'HeatOn' ){
			switch_heat(true);
			AUTO_HEAT	=	false;
		}

	}

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

			if( AUTO_FAN ){
				switch_fan(true);
			}

			if( AUTO_HEAT ){
				switch_heat(true);
			}

		}else if( event.temperature > MAX_TEMPRATURE ){

			digitalWrite(HIGH_TEMP_WARNING_PIN, HIGH);
			digitalWrite(LOW_TEMP_WARNING_PIN, LOW);

			Serial.println("WARNING: High Temprature");

			if( AUTO_FAN ){
				switch_fan(false);
			}

			if( AUTO_HEAT ){
				switch_heat(false);
			}

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
