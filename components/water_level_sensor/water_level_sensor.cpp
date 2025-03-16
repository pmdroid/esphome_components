#include "water_level_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace water_level_sensor {

static const char *TAG = "water_level_sensor";

void WaterLevelSensor::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Water Level Sensor...");
  Wire.begin();
}

void WaterLevelSensor::dump_config() {
  ESP_LOGCONFIG(TAG, "Water Level Sensor:");
  LOG_SENSOR("  ", "Water Level", this);
  ESP_LOGCONFIG(TAG, "  Low section address: 0x%02X", low_addr_);
  ESP_LOGCONFIG(TAG, "  High section address: 0x%02X", high_addr_);
}

void WaterLevelSensor::update() {
  uint8_t highest_section = 0;

  get_low_8_section_value();
  get_high_12_section_value();
 
  // Process low sections
  for (int i = 0; i < 8; i++) {
    if (low_data_[i] > THRESHOLD) {
      if (i > highest_section) highest_section = i;
    }
  }

  // Process high sections
  for (int i = 0; i < 12; i++) {
    if (high_data_[i] > THRESHOLD) {
      if ((i + 8) > highest_section) highest_section = i + 8;
    }
  }
  
  // Calculate water level based on highest section triggered
  float water_level = highest_section * 5;
  ESP_LOGD(TAG, "Water level = %.0f%%", water_level);
  
  publish_state(water_level);
}

void WaterLevelSensor::get_low_8_section_value() {
  memset(low_data_, 0, sizeof(low_data_));
  
  Wire.requestFrom(low_addr_, 8);
  
  uint32_t start_time = millis();
  while (Wire.available() < 8) {
    if (millis() - start_time > 100) {
      ESP_LOGW(TAG, "Timeout waiting for low section data");
      return;
    }
    delay(1);
  }
  
  for (int i = 0; i < 8; i++) {
    low_data_[i] = Wire.read();
  }
  
  delay(10);
}

void WaterLevelSensor::get_high_12_section_value() {
  memset(high_data_, 0, sizeof(high_data_));
  
  Wire.requestFrom(high_addr_, 12);
  
  uint32_t start_time = millis();
  while (Wire.available() < 12) {
    if (millis() - start_time > 100) {
      ESP_LOGW(TAG, "Timeout waiting for high section data");
      return;
    }
    delay(1);
  }
  
  for (int i = 0; i < 12; i++) {
    high_data_[i] = Wire.read();
  }
  
  delay(10);
}

}  // namespace water_level_sensor
}  // namespace esphome