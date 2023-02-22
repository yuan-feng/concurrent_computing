#pragma once
#include <vector>

#include <mpi.h>

class MpiChannel {
public:
  MpiChannel(int otherRank);
  ~MpiChannel();

  int ReceiveInt(int &msg);
  int SendINT(int &msg);
  int ReceiveVecInt(std::vector<int> &msg);
  int SendIntVec(std::vector<int> &msg);

private:
  int other_rank_;
  MPI_Comm other_comm_;
};