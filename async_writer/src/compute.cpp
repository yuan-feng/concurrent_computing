#include <chrono>

#include "compute.h"

Compute::Compute(AsyncWriter *theWriter)
    : writer_(theWriter), current_step_(0), first_step_(true) {
  mtx_ = writer_->mutex();
  writer_->set_step_output_size(STEP_OUTPUT_SIZE);
}

void Compute::StepCompute() {
  // Simulate the compute by a delay.
  std::this_thread::sleep_for(std::chrono::microseconds(1));
  current_step_++;

  this->AsyncWrite();
}

void Compute::AsyncWrite() {
  std::vector<int> id(STEP_OUTPUT_SIZE, current_step_);
  std::vector<float> results(STEP_OUTPUT_SIZE, (float)(current_step_ + 0.5));

  writer_->AsyncWriteId(id);
  writer_->AsyncWriteResults(results);
  
  if (first_step_) {
    std::unique_lock<std::mutex> lk(*mtx_);
    writer_->Notify();
    first_step_ = false;
  }
}