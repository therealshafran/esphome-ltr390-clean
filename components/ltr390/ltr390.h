#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace ltr390 {

class LTR390Component : public PollingComponent {
 public:
  sensor::Sensor *uv_index_sensor{nullptr};
  sensor::Sensor *ambient_light_sensor{nullptr};

  void set_uv_index_sensor(sensor::Sensor *sensor) { uv_index_sensor = sensor; }
  void set_ambient_light_sensor(sensor::Sensor *sensor) { ambient_light_sensor = sensor; }

  void setup() override;
  void update() override;

 protected:
  uint32_t read_register_24(uint8_t reg);
  float calculate_lux(uint32_t raw);
  float calculate_uvi(uint32_t raw);
};

}  // namespace ltr390
}  // namespace esphome
