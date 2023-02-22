#include "shadow_subdomain.h"

#include "shadow_actor_actions.h"

ShadowSubdomain::ShadowSubdomain(int slave_id, MpiMachine &machine,
                                 ObjectBroker &theObjectBroker) {
  channel_ = machine.StartActor(kShadowActorActionCreateSubdomain, slave_id);
  msg_ = std::vector<int>(4);
}

ShadowSubdomain::~ShadowSubdomain() {}

int ShadowSubdomain::HelloWorld() {
  msg_[0] = kShadowActorActionHelloWorld;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::PartitionInputA() {
  msg_[0] = kShadowActorActionPartitionInputA;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::PartitionInputB() {
  msg_[0] = kShadowActorActionPartitionInputB;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::NewStep() {
  msg_[0] = kShadowActorActionNewStep;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::Update() {
  msg_[0] = kShadowActorActionUpdate;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::Compute() {
  msg_[0] = kShadowActorActionCompute;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::CheckResidual() {
  msg_[0] = kShadowActorActionCheckResidual;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::Commit() {
  msg_[0] = kShadowActorActionCommit;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::Revert() {
  msg_[0] = kShadowActorActionRevert;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::CollectResults() {
  msg_[0] = kShadowActorActionCollectResults;
  channel_->SendIntVec(msg_);
  return 0;
}

int ShadowSubdomain::SayBye() {
  msg_[0] = kShadowActorActionSayBye;
  channel_->SendIntVec(msg_);
  return 0;
}
