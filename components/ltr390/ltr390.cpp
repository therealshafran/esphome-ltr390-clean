#include "ltr390.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace ltr390 {

static const char *TAG = "ltr390";

void LTR390Component::setup() {
  ESP_LOGI(TAG, "LTR390 initialized (clean version)");
}

uint32_t LTR390Component::read_register_24(uint8_t reg) {
  uint8_t data[3];
  this->read_bytes(reg, data, 3);
  return (uint32_t(data[2]) << 16) | (uint32_t(data[1]) << 8) | data[0];
}

float LTR390Component::calculate_lux(uint32_t raw) {
  // классическая формула из даташита
  const float gain = 18.0f;
  const float resolution = 20.0f;
  return raw / (gain * resolution);
}

float LTR390Component::calculate_uvi(uint32_t raw) {
  // формула из даташита LTR390
  const float sensitivity = 2300.0f;
  return raw / sensitivity;
}

void LTR390Component::update() {
  uint32_t als_raw = read_register_24(0x0D);
  uint32_t uv_raw = read_register_24(0x10);

  float lux = calculate_lux(als_raw);
  float uvi = calculate_uvi(uv_raw);

  if (ambient_light_sensor != nullptr)
    ambient_light_sensor->publish_state(lux);

  if (uv_index_sensor != nullptr)
    uv_index_sensor->publish_state(uvi);

  ESP_LOGD(TAG, "ALS raw=%u lux=%.3f", als_raw, lux);
  ESP_LOGD(TAG, "UV raw=%u uvi=%.3f", uv_raw, uvi);
}

}  // namespace ltr390
}  // namespace esphome
