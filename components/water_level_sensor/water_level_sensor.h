#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "Wire.h"

namespace esphome {
namespace water_level_sensor {

#define THRESHOLD           100
#define DEFAULT_LOW_ADDR    0x77
#define DEFAULT_HIGH_ADDR   0x78

class WaterLevelSensor : public sensor::Sensor, public PollingComponent {
 public:
  WaterLevelSensor() : PollingComponent(15000) {
    low_addr_ = DEFAULT_LOW_ADDR;
    high_addr_ = DEFAULT_HIGH_ADDR;
  }

  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }
  
  void set_low_address(uint8_t address) { low_addr_ = address; }
  void set_high_address(uint8_t address) { high_addr_ = address; }

 protected:
  void get_low_8_section_value();
  void get_high_12_section_value();

  unsigned char low_data_[8] = {0};
  unsigned char high_data_[12] = {0};
  uint8_t low_addr_;
  uint8_t high_addr_;
};

}  // namespace water_level_sensor
}  // namespace esphome