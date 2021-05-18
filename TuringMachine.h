#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <unistd.h>

class TuringMachine{
private:
  std::unordered_set<char> Q;  // Set of states for the R/W head
  std::unordered_set<char> S;  // Sigma, set of input symbols; subset of G
  std::unordered_set<char> G;  // Gamma, set of tape symbols
  std::unordered_set<char> F;  // F, set of accept states; subset of Q
  char q0;      // q_0, start state of the R/W head; in Q
  char b;       // b, blank tape symbol; in G
  std::string deltaFile; // delta, name of filename to be parsed for transition function

  // A map of "qx" -> "pyD"
  std::unordered_map<std::string, std::string> deltaMap;

  char delta(char q,char x, char out);
  // returns p,y, or L/R for a machine in state q reading x

  // Returns true if set s contains character c
  bool is_in(char c, std::unordered_set<char> s);
  // Adds any character in set c but not in set s to set s
  void add_to(std::unordered_set<char> c, std::unordered_set<char> &s);

public:
  // Constructor
  TuringMachine (std::unordered_set<char> Qi, std::unordered_set<char> Si, std::unordered_set<char> Gi, std::unordered_set<char> Fi,
    char q0i, char bi, std::string deltai);
  // Minimal Constructor
  TuringMachine (std::unordered_set<char> Fi, char q0i, char bi, std::string deltai);
  //MinimalEST Constructor
  TuringMachine (std::string deltai, char bi = '_', char fi = 'f');

  // Run the machine on an input string.
  // init - string that starts on the tape
  // step - controls whether to run it fast or step forward
  bool run(bool step, std::string init);

  //
  void check();
};


// Function implementations

TuringMachine::TuringMachine (std::unordered_set<char> Qi, std::unordered_set<char> Si,
std::unordered_set<char> Gi, std::unordered_set<char> Fi,char q0i, char bi, std::string deltai){
  Q = Qi;
  S = Si;
  G = Gi;
  F = Fi;
  q0 = q0i;
  b = bi;
  deltaFile = deltai;
}

TuringMachine::TuringMachine (std::unordered_set<char> Fi, char q0i, char bi, std::string deltai){
  F = Fi;
  q0 = q0i;
  b = bi;
  deltaFile = deltai;
}

// Most minimal constructor
TuringMachine::TuringMachine (std::string deltai, char bi, char fi){
  std::ifstream deltaIn;
  deltaIn.open(deltai);
  std::string hold;
  bool initialized = false;
  while (getline(deltaIn, hold))
  {
    if (hold[0] != '/' && hold != "") // ignore comments and blank lines
    {
      if (hold[0] == hold[8])
      {
        F.insert(hold[0]); // add p to accept state if it's denoted as such
      }
      if (!initialized)
      {
        q0 = hold[0];
        initialized = true; // start state is set
      }

      Q.insert(hold[0]);

      // Map the input to the output
      std::string in = "", out = "";
      in += hold[0];
      in += hold[2];
      out += hold[4];
      out += hold[6];
      out += hold[8];
      deltaMap[in] = out;

    }
  }
  b = bi;
  F.insert(fi);
  deltaFile = deltai;
}


char TuringMachine::delta(char q,char x,char out){
  // // O(n) on n instructions per step
  // std::ifstream deltaIn;
  // deltaIn.open(deltaFile);
  // // int mod = -1; // Counter to track what part of the function is read
  // std::string hold; // Hold each line that's read in for comparison
  // while (getline(deltaIn, hold))
  // {
  //   if (hold[0] == q)
  //   {
  //     if (hold[2] == x)
  //     { // On the right line of the function document
  //       if (out == 'p') return hold[4]; // New state return
  //       if (out == 'y') return hold[6]; // Write symbol return
  //       if (out == 'D') return hold[8]; // Direction return
  //     }
  //   }
  // }
  // return 'e';

  std::string qx = "";
  qx += q;
  qx += x;
  if (deltaMap.find(qx) == deltaMap.end()) return 'e';
  qx = deltaMap[qx];
  if (out == 'p') return qx[0]; // New state return
  if (out == 'y') return qx[1]; // Write symbol return
  if (out == 'D') return qx[2]; // Direction return

  return 'e';
}

bool TuringMachine::is_in(char c, std::unordered_set<char> s){
  return s.find(c) != s.end();
}

void TuringMachine::add_to(std::unordered_set<char> c, std::unordered_set<char> &s){
  for (std::unordered_set<char>::iterator itr = c.begin(); itr != c.end(); itr++)
  {
    if (!is_in(*itr,s))
      s.insert(*itr);
  }
}

bool TuringMachine::run(bool step, std::string init){
  std::string tape = q0 + init; // String to represent the tape (instantaneous defiition)
  char q = q0;  // State of the RW head
  int cell = 1; // Cell looked at by writing head
  char y;       // Symbol to write
  char d;       // Direction to move

  std::cout << "\033[2J\033[2;1H";
  std::cout << deltaFile << std::endl << std::endl;
  std::cout << tape.substr(0,cell-1) << '(' << tape[cell-1] << ')' << tape.substr(cell) << '\n';
  if (step) usleep(1000000);

  while (!is_in(q,F)){
    // The contents of this loop represent one move of the TM
    if (cell >= tape.size())
      tape += b; // Expand tape right if needed
    if (cell == 1){
      tape = b + tape;
      cell++; // Expanding left is really a sort of "shifting" in the string
    }

    // Read state, symbol, and direction from transition function
    y = delta(q,tape[cell],'y');
    d = delta(q,tape[cell],'D');
    q = delta(q,tape[cell],'p');
    if (q == 'e'){ // Check for a reject state
      std::cout << "reject" << std::endl;
      return false;
    }
    // Continue reading transition data

    // Move head and update tape
    // This string manipulation updates the instantaneous description of the TM for printing
    if (d == 'L'){ // Move left on an L
      tape = tape.substr(0,cell-2) + q + tape[cell-2] + y + tape.substr(cell+1);
      cell--;
    }
    else { // Move right
      tape = tape.substr(0,cell-1) + y + q + tape.substr(cell+1);
      cell++;
    }

    std::cout << tape.substr(0,cell-1) << '(' << tape[cell-1] << ')' << tape.substr(cell) << '\n';
    if (step) usleep(1000000);
  }
  std::cout << "accept" << std::endl;
  return true;
}

void TuringMachine::check(){
  // Ensure the blank symbol is in the tape alphabet
  if (!is_in(b,G))
    G.insert(b);
  // Ensure the start state is in the set of states
  if (!is_in(q0,Q))
    Q.insert(q0);
  // Ensure the set of accept states is a subset of the set of states
  add_to(F,Q);
  // Ensure the input alphabet is a subset of the tape alphabet
  add_to(S,G);

  if (is_in('e',Q) || is_in('e',G) || is_in('e',F))
  {
    std::cout << "Warning: exit symbol 'e' possibly misused";
  }
  if (is_in('L',Q) || is_in('R',Q))
  {
    std::cout << "Warning: direction symbol in Q, might misinterpret accept state";
  }

  // std::cout << deltaFile <<'\n';
  // for (auto x : deltaMap)
  // {
  //   std::cout << x.first << " " << x.second << std::endl;
  // }
}
