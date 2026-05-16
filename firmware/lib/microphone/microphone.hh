#ifndef MICROPHONE_HH
#define MICROPHONE_HH

#include <Arduino.h>
#include "driver/i2s_std.h"

class Microphone {
  static constexpr uint16_t BUFFER_SIZE = 256;
  static constexpr uint16_t SAMPLE_RATE = 16000;
  static constexpr double MAX_INT32_T = std::numeric_limits<int32_t>::max();
  static constexpr uint8_t DB_OFFSET = 100;
  i2s_chan_handle_t rx_handle;
  int32_t buffer[BUFFER_SIZE];
  int32_t samples_read;
  double calculate_rms(); 
  const double get_decibels();
public:
  Microphone(int32_t pin_sck, int32_t pin_ws, int32_t pin_sd);
  // inline const int32_t* get_buffer() const { return buffer; }
  // inline size_t get_buffer_sample_count() const { return samples_read; }
  int32_t read_raw_data_to_buffer();
  const String get_json_packet(const String &device_id);
};

#endif
