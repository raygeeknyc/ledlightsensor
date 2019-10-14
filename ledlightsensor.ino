/**
 * Use a CDS photosensor to read light levels and PWM connected LED to show changes.
 * Written for an Adafruit Pro Trinket.
 * 
 * 2019 by Raymond Blum <raygeeknyc@gmail.com>.
 */

#define PIN_LED 5
#define PIN_CDS A0

#define SENSOR_SAMPLES 5
#define MAX_SENSOR_READING 1023

int current_light_level;
int previous_light_level;

#define LIGHT_CHANGE_THRESHOLD 50
#define LED_STEP 30
#define LED_MAX_VALUE 255
#define LED_MIN_VALUE 0

int led_brightness;

#define SAMPLE_DELAY_MS 500

int smooth(int array[], int len) {
  /**
    Return the average of the array without the highest and lowest values.
  **/
  int low = MAX_SENSOR_READING;
  int high = -1;
  int total = 0;
  for (int s = 0; s < len; s++) {
    total += array[s];
    low = min(array[s], low);
    high = max(array[s], high);
  }
  total -= low;
  total -= high;
  return total / (len - 2);
}

int getLightLevel() {
  // Return the median reading from the light sensor
  int samples[SENSOR_SAMPLES];
  for (int sample = 0; sample < SENSOR_SAMPLES; sample++) {
    samples[sample] = analogRead(PIN_CDS);
  }
  return smooth(samples, SENSOR_SAMPLES);
}

void setup() {
 current_light_level = MAX_SENSOR_READING;
 previous_light_level = MAX_SENSOR_READING;
 led_brightness = (LED_MAX_VALUE + LED_MIN_VALUE) / 2;
}

void loop() {
  current_light_level = getLightLevel();
  if (current_light_level > previous_light_level + LIGHT_CHANGE_THRESHOLD) {
    led_brightness = max(led_brightness+LED_STEP, LED_MAX_VALUE);
    previous_light_level = current_light_level;
  } else if (current_light_level < previous_light_level - LIGHT_CHANGE_THRESHOLD) {
    led_brightness = min(led_brightness-LED_STEP, LED_MIN_VALUE);
    previous_light_level = current_light_level;
  }
  analogWrite(PIN_LED, led_brightness);
  delay(SAMPLE_DELAY_MS);
}
