#pragma once
#include <vector>

#include <mpi.h>

#include "domain.h"
#include "mpi_channel.h"
#include "mpi_machine.h"
#include "object_broker.h"

class MpiMachine;
class ObjectBroker;
class MpiChannel;

class ShadowSubdomain : public Domain {
public:
  ShadowSubdomain(int tag, MpiMachine &machine, ObjectBroker &theObjectBroker);
  virtual ~ShadowSubdomain();
  int HelloWorld();
  int PartitionInputA();
  int PartitionInputB();
  int NewStep();
  int Update();
  int Compute();
  int CheckResidual();
  int Commit();
  int Revert();
  int CollectResults();
  int SayBye();

private:
  MpiChannel *channel_;
  std::vector<int> msg_;
};