#include "TuringMachine.h"

int main(){

  // States
  std::set<char> Q; Q.insert('A'); Q.insert('B'); Q.insert('C');
  // Input symbols
  std::set<char> S; S.insert('0'); S.insert('1');
  // Tape symbols
  std::set<char> G(Q); G.insert('$'); G.insert ('_');
  // Accept states
  std::set<char> F; F.insert('C');
  // Start state
  char q0 = 'A';
  // Blank symbol
  char b = '_';
  // Delta function infile
  std::string deltaFile = "binaryIncrement.txt";

  TuringMachine M (Q,S,G,F,q0,b,deltaFile);
  M.run(true, "$10111");

}
