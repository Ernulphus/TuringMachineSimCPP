#include "TuringMachine.h"
#include <iostream>
#include <unistd.h>

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

    char TuringMachine::delta(std::string d,char q,char x,char out){
      std::ifstream deltaIn;
      deltaIn.open(d);
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
        p = delta(deltaFile,q,tape[cell],'p');
        y = delta(deltaFile,q,tape[cell],'y');
        d = delta(deltaFile,q,tape[cell],'D');

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

/*
    TuringMachine delta_p(char d,char q,char x){

    }

    TuringMachinechar delta_x(char d,char q,char x){

    }

    TuringMachinechar delta_D(char d, char q, char x){

    }
*/

int main(){
  std::set<char> Q; Q.insert('A'); Q.insert('B'); Q.insert('C');
  std::set<char> S; S.insert('0'); S.insert('1');
  std::set<char> G(Q); G.insert('$'); G.insert ('_');
  std::set<char> F; F.insert('C');
  char q0 = 'A';
  char b = '_';
  std::string deltaFile = "delta.txt";

  TuringMachine M (Q,S,G,F,q0,b,deltaFile);
  M.run(true, "$11");

}
