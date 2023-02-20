#include <iostream>
#include <thread>
#include <vector>

void minionRollCall(int n) {
  std::cout << "Hello! I am minion " << n << std::endl;
}

void overlordGreet() {
  std::cout << "Hello Minions! I am the Overlord." << std::endl;
}

int main (int argc, char** argv) {
  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbminions>\n";
    return -1;
  }
  //Your code goes here
  int nbminions;
  sscanf(argv[1], "%d", &nbminions); //Stores number of minions in integer

  std::vector<std::thread> minions; //Create vector to store minions

  for(int i = 1; i <= nbminions; ++i) {
    std::thread minion (minionRollCall, i); //Minions announce themselves on creation

    minions.push_back(std::move(minion));   //Store minion threads into vector by moving (NOT copying) memory
  }

  for (auto & m : minions) {
    if (m.joinable()) {                     //Join all minions
      m.join();
    }
    else {
      std::cout << "Minion was unable to join." << std::endl;
    }
  }

  // Use thread for Overlord
  std::thread overlord (overlordGreet);
  if (overlord.joinable()){
    overlord.join();
  }

  return 0;
}
