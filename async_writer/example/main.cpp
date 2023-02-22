#include <async_writer.h>
#include <compute.h>

int main(int argc, char const *argv[]) {
  std::mutex mtx;
  std::string filename = "test_async";
  int total_steps = 10;
  bool async_output_is_enabled = true;

  std::cout << "start async_writer " << std::endl;
  AsyncWriter async_writer(&mtx, filename);
  async_writer.SwitchAsyncOutput(async_output_is_enabled);

  std::cout << "start compute" << std::endl;
  Compute compute(&async_writer);
  compute.set_total_output_step(total_steps);

  std::cout << "start writing " << std::endl;
  async_writer.StartWriting();

  std::cout << "start compute new steps" << std::endl;
  for (int i = 0; i < total_steps; ++i) {
    compute.StepCompute();
  }

  std::cout << "\n";
  std::cout << "*********************\n";
  std::cout << "**Computation Done!**\n";
  std::cout << "*********************\n";
  return 0;
}