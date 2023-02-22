#include "async_writer.h"

AsyncWriter::AsyncWriter(std::mutex *mtx, const std::string& filename)
    : async_output_is_enabled_(false), mtx_(mtx), filename_(filename),
      current_output_step_(0) {
  output_id_buffer_.set_capacity(sizeof(int) *
                                 CONCURRENT_BOUNDED_QUEUE_CAPACITY);
  output_results_buffer_.set_capacity(sizeof(float) *
                                      CONCURRENT_BOUNDED_QUEUE_CAPACITY);
  ofstream_id_.open(filename_ + "_id.txt",
                    std::ofstream::out | std::ofstream::app);
  ofstream_result_.open(filename_ + "_result.txt",
                        std::ofstream::out | std::ofstream::app);
  cv_ = new std::condition_variable();
}

AsyncWriter::~AsyncWriter() {

  ofstream_id_.flush();
  ofstream_result_.flush();

  if (async_output_is_enabled_ && async_writer_.joinable()) {
    async_writer_.join();
  }
  std::cout << "\n";
  std::cout << "*********************\n";
  std::cout << "****Output  Done!****\n";
  std::cout << "*********************\n";
}

void AsyncWriter::Notify() { cv_->notify_one(); }

void AsyncWriter::StartWriting() {
  if (async_output_is_enabled_) {
    if (async_writer_.joinable()) {
      async_writer_.join();
    }
    async_writer_ = std::thread([=] {
      std::unique_lock<std::mutex> lk(*mtx_);
      cv_->wait(lk);
      WriteToDisk();
    });
  }
}

void AsyncWriter::WriteToDisk() {
  while (current_output_step_ < total_output_step_) {
    this->WriteIdToDisk(current_output_step_);
    this->WriteResultsToDisk(current_output_step_);
    current_output_step_++;
  }
}

void AsyncWriter::SwitchAsyncOutput(bool b) { async_output_is_enabled_ = b; }

void AsyncWriter::AsyncWriteId(const std::vector<int>& id) {
  for (int i = 0; i < step_output_size_; ++i) {
    output_id_buffer_.push(id[i]);
  }
}

void AsyncWriter::AsyncWriteResults(const std::vector<float>& results) {
  for (int i = 0; i < step_output_size_; ++i) {
    output_results_buffer_.push(results[i]);
  }
}

void AsyncWriter::WriteIdToDisk(int step) {
  for (int i = 0; i < step_output_size_; ++i) {
    int item = -1;
    output_id_buffer_.pop(item);
    std::cout << item << " ";
    ofstream_id_ << item << " ";
  }
  std::cout << std::endl;
  ofstream_id_ << std::endl;
}

void AsyncWriter::WriteResultsToDisk(int step) {
  for (int i = 0; i < step_output_size_; ++i) {
    float item = -1;
    output_results_buffer_.pop(item);
    std::cout << item << " ";
    ofstream_result_ << item << " ";
  }
  std::cout << std::endl;
  ofstream_result_ << std::endl;
}
