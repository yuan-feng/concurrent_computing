#include "mpi_machine.h"

#include "shadow_actor_actions.h"

MpiMachine::MpiMachine(ObjectBroker *broker_, int argc, char **argv)
    : broker_(broker_) {
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_id_);
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc_);
  channels_ = std::vector<MpiChannel *>(num_proc_);

  for (int i = 0; i < num_proc_; i++) {
    channels_[i] = new MpiChannel(i);
  }
}

MpiMachine::~MpiMachine() { MPI_Finalize(); }
int MpiMachine::GetProcessID() { return proc_id_; }

int MpiMachine::GetNumProcess() { return num_proc_; }

int MpiMachine::RunActor() {
  Mout << "MpiMachine::RunActor " << std::endl;
  int done = 0;
  int msg = -1;
  // loop until recv kill signal
  while (!done) {
    if (0 != channels_[0]->ReceiveInt(msg)) {
      std::cerr << "ERROR! MpiMachine::RunActor failed to recv actor_type "
                << std::endl;
      return -1;
    }
    int actor_type = msg;
    if (msg == kShadowActorActionShutdownSlaveProcess) {
      Mout << "    Finalizing current actor " << std::endl;
      done = 1;
      if (0 != channels_[0]->SendINT(done)) {
        std::cerr << "ERROR! MpiMachine::RunActor failed to send done status "
                  << std::endl;
        return -1;
      }
    } else {
      ActorSubdomain *actor = broker_->GetNewActor(actor_type, channels_[0]);
      if (!actor) {
        std::cerr << "ERROR! MpiMachine::RunActor failed to GetNewActor "
                  << std::endl;
        return -1;
      }
      int created = 1;
      if (0 != channels_[0]->SendINT(created)) {
        std::cerr << "MachineBroker::Run(void) - failed to send ID\n";
        return -1;
      }
      if (0 != actor->Run()) {
        std::cerr << "MachineBroker::Run(void) - actor failed while running\n";
        return -1;
      }
      delete actor;
    }
  }
  Mout << "Actor Done!" << std::endl;
  return 0;
}

MpiChannel *MpiMachine::StartActor(int actor_type, int slave_id) {
  Mout << "MpiMachine::StartActor " << std::endl;
  int msg = actor_type;
  if (0 != channels_[slave_id]->SendINT(msg)) {
    std::cerr << "ERROR! MpiMachine::StartActor failed to send actor_type"
              << std::endl;
    return nullptr;
  }
  int remote_actor_started = 0;
  if (0 != channels_[slave_id]->ReceiveInt(remote_actor_started)) {
    std::cerr << "MpiMachine::StartActor() - failed recv remote actor # "
              << actor_type << "start status!" << std::endl;
    return nullptr;
  }
  if (!remote_actor_started) {
    std::cerr
        << "MpiMachine::StartActor() - could not start remote actor_type # "
        << actor_type << std::endl;
    return nullptr;
  }
  return channels_[slave_id];
}

int MpiMachine::Shutdown(int slave_id) {
  Mout << "MpiMachine::Shutdown " << std::endl;
  int msg = kShadowActorActionShutdownSlaveProcess;
  if (0 != channels_[slave_id]->SendINT(msg)) {
    std::cerr << "ERROR! MpiMachine::Shutdown failed to send Shutdown"
              << std::endl;
    return -1;
  }
  int remote_actor_closed = 0;
  if (0 != channels_[slave_id]->ReceiveInt(remote_actor_closed)) {
    std::cerr
        << "MpiMachine::Shutdown() - failed to recv close remote actor status "
        << std::endl;
    return -1;
  }
  if (!remote_actor_closed) {
    std::cerr << "MpiMachine::Shutdown() - could not closed remote actor "
              << std::endl;
    return -1;
  }
  return 0;
}