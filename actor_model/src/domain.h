#pragma once
#include <iostream>
#include <vector>

#include <mpi.h>

#define DemoMSG std::cout << "Domain(" << rank << "): "

class Domain {
public:
  Domain();
  virtual ~Domain();

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

private:
  int rank;
};