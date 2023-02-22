#pragma once
#include <vector>

#include <mpi.h>

#include "domain.h"
#include "mpi_channel.h"
#include "object_broker.h"

class MpiChannel;
class ObjectBroker;

class ActorSubdomain : public Domain {
public:
  ActorSubdomain(MpiChannel *channel__, ObjectBroker *broker_);
  virtual ~ActorSubdomain();
  int Run();

private:
  MpiChannel *channel_;
  ObjectBroker *broker_;
  std::vector<int> msg_;
};