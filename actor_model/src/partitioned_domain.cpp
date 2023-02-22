#include "partitioned_domain.h"

int PartitionedDomain::AddSubdomain(ShadowSubdomain *sub) {
  subdomains_.push_back(sub);
  return 0;
}

int PartitionedDomain::HelloWorld() {
  std::cout << " PartitionedDomain::HelloWorld " << std::endl;
  this->Domain::HelloWorld();
  for (auto sub : subdomains_) {
    sub->HelloWorld();
  }
  return 0;
}

int PartitionedDomain::PartitionInputA() {
  this->Domain::PartitionInputA();
  for (auto sub : subdomains_) {
    sub->PartitionInputA();
  }
  return 0;
}

int PartitionedDomain::PartitionInputB() {
  this->Domain::PartitionInputB();
  for (auto sub : subdomains_) {
    sub->PartitionInputB();
  }
  return 0;
}

int PartitionedDomain::NewStep() {
  this->Domain::NewStep();
  for (auto sub : subdomains_) {
    sub->NewStep();
  }
  return 0;
}

int PartitionedDomain::Update() {
  this->Domain::Update();
  for (auto sub : subdomains_) {
    sub->Update();
  }
  return 0;
}

int PartitionedDomain::Compute() {
  this->Domain::Compute();
  for (auto sub : subdomains_) {
    sub->Compute();
  }
  return 0;
}

int PartitionedDomain::CheckResidual() {
  this->Domain::CheckResidual();
  for (auto sub : subdomains_) {
    sub->CheckResidual();
  }
  return 0;
}

int PartitionedDomain::Commit() {
  this->Domain::Commit();
  for (auto sub : subdomains_) {
    sub->Commit();
  }
  return 0;
}

int PartitionedDomain::Revert() {
  this->Domain::Revert();
  for (auto sub : subdomains_) {
    sub->Revert();
  }
  return 0;
}

int PartitionedDomain::CollectResults() {
  this->Domain::CollectResults();
  for (auto sub : subdomains_) {
    sub->CollectResults();
  }
  return 0;
}

int PartitionedDomain::SayBye() {
  this->Domain::SayBye();
  for (auto sub : subdomains_) {
    sub->SayBye();
  }
  return 0;
}
