#pragma once
#include "async_writer.h"

#define STEP_OUTPUT_SIZE 50

class Compute {
public:
  Compute(AsyncWriter *theWriter);

  void StepCompute();
  void set_total_output_step(int step) {
    writer_->set_total_output_step(step);
  }

private:
  void AsyncWrite();

  AsyncWriter *writer_;
  std::mutex *mtx_;
  int current_step_;
  bool first_step_;
};