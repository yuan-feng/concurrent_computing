#pragma once

#include <vector>

#include "mpi_channel.h"
#include "object_broker.h"

class ObjectBroker;
class MpiChannel;

#define Mout std::cout << "MpiMachine(" << proc_id_ << "): "
class MpiMachine {
public:
  MpiMachine(ObjectBroker *broker, int argc, char **argv);
  ~MpiMachine();

  int GetProcessID();
  int GetNumProcess();
  // For slave processes
  int RunActor();

  // For the shadow of the master process.
  MpiChannel *StartActor(int actor_type, int slave_id);
  int Shutdown(int slave_id);

private:
  int proc_id_;
  int num_proc_;
  std::vector<MpiChannel *> channels_;
  ObjectBroker *broker_;
};