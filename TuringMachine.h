#include <set>
#include <fstream>
#include <iostream>
#include <unistd.h>

class TuringMachine{
private:
  std::set<char> Q;  // Set of states for the R/W head
  std::set<char> S;  // Sigma, set of input symbols; subset of G
  std::set<char> G;  // Gamma, set of tape symbols
  std::set<char> F;  // F, set of accept states; subset of Q
  char q0;      // q_0, start state of the R/W head; in Q
  char b;       // b, blank tape symbol; in G
  std::string deltaFile; // delta, name of filename to be parsed for transition function

  char delta(char q,char x, char out);
  // returns p,y, or L/R for a machine in state q reading x

  // Returns true if set s contains character c
  bool is_in(char c, std::set<char> s);
  // Adds any character in set c but not in set s to set s
  void add_to(std::set<char> c, std::set<char> &s);

public:
  // Constructor
  TuringMachine (std::set<char> Qi, std::set<char> Si, std::set<char> Gi, std::set<char> Fi,
    char q0i, char bi, std::string deltai);
  // Minimal Constructor
  TuringMachine (std::set<char> Fi, char q0i, char bi, std::string deltai);

  // Run the machine on an input string.
  // init - string that starts on the tape
  // step - controls whether to run it fast or step forward
  bool run(bool step, std::string init);

  //
  void check();
};


// Function implementations

TuringMachine::TuringMachine (std::set<char> Qi, std::set<char> Si,
std::set<char> Gi, std::set<char> Fi,char q0i, char bi, std::string deltai){
  Q = Qi;
  S = Si;
  G = Gi;
  F = Fi;
  q0 = q0i;
  b = bi;
  deltaFile = deltai;
}

TuringMachine::TuringMachine (std::set<char> Fi, char q0i, char bi, std::string deltai){
  F = Fi;
  q0 = q0i;
  b = bi;
  deltaFile = deltai;
}


char TuringMachine::delta(char q,char x,char out){
  std::ifstream deltaIn;
  deltaIn.open(deltaFile);
  int mod = -1; // Counter to track what part of the function is read
  char hold; // Hold each char that's read in for comparison
  while (deltaIn >> hold)
  {
    mod++;
    if (mod%5 == 0 && hold == q)
    {
      deltaIn >> hold;
      mod++;
      if (hold == x)
      { // On the right line of the function document
        deltaIn >> hold;
        if (out == 'p') return hold; // New state return
        deltaIn >> hold;
        if (out == 'y') return hold; // Write symbol return
        deltaIn >> hold;
        if (out == 'D') return hold; // Direction return
      }
    }
  }
  return 'e';
}

bool TuringMachine::is_in(char c, std::set<char> s){
  return s.find(c) != s.end();
}

void add_to(std::set<char> c, std::set<char> s){
  for (itr = c.begin(); itr != c.end(); itr++)
  {
    if (!is_in(*itr,s))
      s.insert(*itr);
  }
}

bool TuringMachine::run(bool step, std::string init){
  std::string tape = q0 + init; // String to represent the tape
  char q = q0;  // State of the RW head
  int cell = 1; // Cell looked at by writing head
  char p;       // State to change to
  char y;       // Symbol to write
  char d;       // Direction to move

  std::cout << "\033[2J\033[2;2H";
  std::cout << tape + '\n';
  if (step) usleep(2500000);

  while (!is_in(q,F)){
    // The contents of this loop represent one move of the TM
    if (cell >= tape.size()) tape += b; // Expand tape if needed
    // Read and get transition data
    p = delta(q,tape[cell],'p');
    y = delta(q,tape[cell],'y');
    d = delta(q,tape[cell],'D');

    // Move head and update tape
    // This string manipulation updates the instantaneous description of the TM for printing
    if (d == 'L' && cell > 0){ // Move left on an L unless we're at the start of the tape
      tape = tape.substr(0,cell-2) + p + tape[cell-2] + y + tape.substr(cell+1);
      cell--;
    }
    else { // Move right
      tape = tape.substr(0,cell-1) + y + p + tape.substr(cell+1);
      cell++;
    }

    q = p; // Update state

    std::cout << "\033[2J\033[2;2H";
    std::cout << tape + '\n';
    if (step) usleep(2500000);
  }

  return true;
}

void TuringMachine::check(){
  // Ensure the blank symbol is in the tape alphabet
  if (!is_in(b,G))
    G.insert(b);
  // Ensure the start state is in the set of states
  if (!is_in(q0,Q)
    Q.insert(q0);
  // Ensure the set of accept states is a subset of the set of states
  add_to(F,S);
  // Ensure the input alphabet is a subset of the tape alphabet
  add_to(S,G);
}
