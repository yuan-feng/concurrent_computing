#pragma once
#include <atomic>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <tbb/tbb.h>

#define CONCURRENT_BOUNDED_QUEUE_CAPACITY 100

class AsyncWriter {
public:
  AsyncWriter(std::mutex *mtx, const std::string& filename);
  ~AsyncWriter();

  void StartWriting();
  void Notify();
  void SwitchAsyncOutput(bool b);
  void AsyncWriteId(const std::vector<int>& id);
  void AsyncWriteResults(const std::vector<float>& results);

  void set_step_output_size(int stepsize) { step_output_size_ = stepsize; }
  void set_total_output_step(int step) { total_output_step_ = step; }

  std::mutex *mutex() { return mtx_; }

private:
  void WriteIdToDisk(int step);
  void WriteResultsToDisk(int step);
  void WriteToDisk();

  bool async_output_is_enabled_;
  std::mutex *mtx_;
  std::string filename_;
  std::condition_variable *cv_;
  std::ofstream ofstream_id_;
  std::ofstream ofstream_result_;
  int step_output_size_;

  tbb::concurrent_bounded_queue<int> output_id_buffer_;
  tbb::concurrent_bounded_queue<float> output_results_buffer_;
  std::thread async_writer_;
  int current_output_step_;
  int total_output_step_;
};