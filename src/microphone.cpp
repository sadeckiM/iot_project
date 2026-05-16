#include "microphone.hh"
#include "driver/i2s_common.h"
#include <ArduinoJson.h>

Microphone::Microphone(int32_t pin_sck, int32_t pin_ws, int32_t pin_sd) {
  i2s_chan_config_t chan_config = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0,
                                                          I2S_ROLE_MASTER);
  i2s_new_channel(&chan_config, NULL, &rx_handle);

  i2s_std_config_t std_cfg = {
    .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
    .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT,
                                                I2S_SLOT_MODE_MONO),
    .gpio_cfg = {
      .mclk = I2S_GPIO_UNUSED,
      .bclk = static_cast<gpio_num_t>(pin_sck),
      .ws = static_cast<gpio_num_t>(pin_ws),
      .dout = I2S_GPIO_UNUSED,
      .din = static_cast<gpio_num_t>(pin_sd)
    }
  };
  std_cfg.slot_cfg.slot_mask = I2S_STD_SLOT_LEFT;

  i2s_channel_init_std_mode(rx_handle, &std_cfg);
  i2s_channel_enable(rx_handle);
}

int32_t Microphone::read_raw_data_to_buffer() {
  size_t bytes_read = 0;
  esp_err_t err = i2s_channel_read(rx_handle, buffer, sizeof(int32_t) *
                                   BUFFER_SIZE, &bytes_read, pdMS_TO_TICKS(100)); 
  if (err != ESP_OK) {
    Serial.printf("I2S: read_raw: %s\n", esp_err_to_name(err));
    samples_read = 0;
    return -1;
  }
  samples_read = static_cast<int32_t>(bytes_read / sizeof(int32_t));
  return samples_read;
}

double Microphone::calculate_rms() {
  if (samples_read == 0) return 0.0;

  double sum = 0;
  for (uint16_t i = 0; i < samples_read; ++i) {
    double normalized_sample = static_cast<double>(buffer[i] / MAX_INT32_T);
    sum += normalized_sample * normalized_sample;
  }
  return std::sqrt(sum / samples_read);
}

const double Microphone::get_decibels() { 
  double rms = calculate_rms();
  if (rms < 0.00001) return 0.0;
  return 20 * log10(rms) + DB_OFFSET;
}

const String Microphone::get_json_packet(const String &device_id) {
  double current_db = get_decibels();
  JsonDocument doc;

  doc["device"] = device_id;
  doc["db"] = current_db;

  String json_string;
  serializeJson(doc, json_string);
  return json_string;
}
