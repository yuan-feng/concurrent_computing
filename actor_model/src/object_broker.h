#pragma once
#include <vector>

#include <mpi.h>

#include "actor_subdomain.h"
#include "mpi_channel.h"

class MpiChannel;
class ActorSubdomain;

class ObjectBroker {
public:
  ActorSubdomain *GetNewActor(int action_tag, MpiChannel *channel_);
};