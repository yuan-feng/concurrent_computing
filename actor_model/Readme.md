## Actor Model in C++
This is a practice implementation of actor model prototype in C++ and MPI. The actor model in Computer science is a mathematical model of concurrent computation that treats an actor as the basic building block of concurrent computation. In response to a message it receives, an actor can: make local decisions, create more actors, send more messages, and determine how to respond to the next message received. Actors may modify their own private state, but can only affect each other indirectly through messaging (removing the need for lock-based synchronization).

#### Installation
On Ubuntu
```bash
sudo apt install libopenmpi-dev
```


#### Compile
```bash
cd ./example && make
```

#### Run the example
```bash
./run_sequential.sh
./run_parallel.sh
```




