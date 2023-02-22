#include "mpi_channel.h"

constexpr int GLOBAL_MSG_TAG = 99;

MpiChannel::MpiChannel(int other_tag)
    : other_rank_(other_tag), other_comm_(MPI_COMM_WORLD) {}

int MpiChannel::SendINT(int &msg) {
  int *data = &msg;
  char *g_msg = (char *)data;
  MPI_Send((void *)g_msg, 1, MPI_INT, other_rank_, GLOBAL_MSG_TAG, other_comm_);
  return 0;
}

int MpiChannel::ReceiveInt(int &msg) {
  int *data = &msg;
  char *g_msg = (char *)data;
  MPI_Status status;
  MPI_Recv((void *)g_msg, 1, MPI_INT, other_rank_, GLOBAL_MSG_TAG, other_comm_,
           &status);
  return 0;
}

int MpiChannel::SendIntVec(std::vector<int> &msg) {
  int *data = &msg[0];
  int sz = msg.size();
  char *g_msg = (char *)data;
  MPI_Send((void *)g_msg, sz, MPI_INT, other_rank_, GLOBAL_MSG_TAG,
           other_comm_);
  return 0;
}

int MpiChannel::ReceiveVecInt(std::vector<int> &msg) {
  int *data = &msg[0];
  int sz = msg.size();
  char *g_msg = (char *)data;
  MPI_Status status;
  MPI_Recv((void *)g_msg, sz, MPI_INT, other_rank_, GLOBAL_MSG_TAG, other_comm_,
           &status);
  return 0;
}
