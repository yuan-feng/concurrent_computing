#pragma once
#include <vector>

#include <mpi.h>

#include "domain.h"
#include "shadow_subdomain.h"

class ShadowSubdomain;

class PartitionedDomain final : public Domain {
public:
  virtual int HelloWorld();
  virtual int PartitionInputA();
  virtual int PartitionInputB();
  virtual int NewStep();
  virtual int Update();
  virtual int Compute();
  virtual int CheckResidual();
  virtual int Commit();
  virtual int Revert();
  virtual int CollectResults();
  virtual int SayBye();

  int AddSubdomain(ShadowSubdomain *sub);

private:
  std::vector<ShadowSubdomain *> subdomains_;
};