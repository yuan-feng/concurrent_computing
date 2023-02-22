#include "actor_subdomain.h"

#include "shadow_actor_actions.h"

ActorSubdomain::ActorSubdomain(MpiChannel *channel, ObjectBroker *broker)
    : channel_(channel), broker_(broker) {}

ActorSubdomain::~ActorSubdomain() {}

int ActorSubdomain::Run() {
  bool done = false;
  msg_ = std::vector<int>(4);

  while (!done) {
    channel_->ReceiveVecInt(msg_);
    int action = msg_[0];
    switch (action) {
    case kShadowActorActionHelloWorld: {
      this->HelloWorld();
      break;
    }
    case kShadowActorActionPartitionInputA: {
      this->PartitionInputA();
      break;
    }
    case kShadowActorActionPartitionInputB: {
      this->PartitionInputB();
      break;
    }
    case kShadowActorActionNewStep: {
      this->NewStep();
      break;
    }
    case kShadowActorActionUpdate: {
      this->Update();
      break;
    }
    case kShadowActorActionCompute: {
      this->Compute();
      break;
    }
    case kShadowActorActionCheckResidual: {
      this->CheckResidual();
      break;
    }
    case kShadowActorActionCommit: {
      this->Commit();
      break;
    }
    case kShadowActorActionRevert: {
      this->Revert();
      break;
    }
    case kShadowActorActionCollectResults: {
      this->CollectResults();
      break;
    }
    case kShadowActorActionSayBye: {
      this->SayBye();
      done = true;
      break;
    }
    default: {
      std::cerr << " ERROR! ActorSubdomain::Run() Unknown Action! ";
      std::cerr << " actionTag = " << action << std::endl;
      return -1;
    }
    }
  }
  return 0;
}
