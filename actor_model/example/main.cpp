#include "mpi_machine.h"
#include "object_broker.h"
#include "partitioned_domain.h"
#include "shadow_subdomain.h"

int main(int argc, char *argv[]) {
  ObjectBroker broker;
  MpiMachine machine(&broker, argc, argv);
  const int rank = machine.GetProcessID();
  const int np = machine.GetNumProcess();
  PartitionedDomain master_domain;
  if (rank != 0) {
    machine.RunActor();
  } else {
    const int num_subdomains = np - 1;
    for (int i = 1; i <= num_subdomains; i++) {
      ShadowSubdomain *sub_domain = new ShadowSubdomain(i, machine, broker);
      if (0 != master_domain.AddSubdomain(sub_domain)) {
        std::cerr << "   Failed to add Subdomain # " << i << " !\n";
        return -1;
      }
      std::cout << "   Subdomain # " << i << " created. \n";
    }

    master_domain.HelloWorld();
    master_domain.PartitionInputA();
    master_domain.PartitionInputB();

    master_domain.NewStep();
    master_domain.Update();
    master_domain.Compute();
    master_domain.CheckResidual();
    master_domain.Commit();
    master_domain.Revert();

    master_domain.CollectResults();
    master_domain.SayBye();

    for (int i = 1; i <= num_subdomains; ++i) {
      machine.Shutdown(i);
    }
  }

  return 0;
}