#include "domain.h"

Domain::Domain() { MPI_Comm_rank(MPI_COMM_WORLD, &rank); }
Domain::~Domain() {}

int Domain::HelloWorld() {
  DemoMSG << "Domain::HelloWorld " << std::endl;
  return 0;
}

int Domain::PartitionInputA() {
  DemoMSG << "Domain::PartitionInputA " << std::endl;
  return 0;
}

int Domain::PartitionInputB() {
  DemoMSG << "Domain::PartitionInputB " << std::endl;
  return 0;
}

int Domain::NewStep() {
  DemoMSG << "Domain::NewStep " << std::endl;
  return 0;
}

int Domain::Update() {
  DemoMSG << "Domain::Update " << std::endl;
  return 0;
}

int Domain::Compute() {
  DemoMSG << "Domain::Compute " << std::endl;
  return 0;
}

int Domain::CheckResidual() {
  DemoMSG << "Domain::CheckResidual " << std::endl;
  return 0;
}

int Domain::Commit() {
  DemoMSG << "Domain::Commit " << std::endl;
  return 0;
}

int Domain::Revert() {
  DemoMSG << "Domain::Revert " << std::endl;
  return 0;
}

int Domain::CollectResults() {
  DemoMSG << "Domain::CollectResults " << std::endl;
  return 0;
}

int Domain::SayBye() {
  DemoMSG << "Domain::SayBye " << std::endl;
  return 0;
}
