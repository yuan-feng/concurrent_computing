#include "object_broker.h"

#include "shadow_actor_actions.h"

ActorSubdomain *ObjectBroker::GetNewActor(int action_tag, MpiChannel *channel) {
  switch (action_tag) {
  case kShadowActorActionCreateSubdomain: {
    return new ActorSubdomain(channel, this);
  }
  default: {
    std::cerr << "ObjectBroker::GetNewActor - ";
    std::cerr << " - no action_tag exists for " << action_tag << std::endl;
    return nullptr;
  }
  }
}
