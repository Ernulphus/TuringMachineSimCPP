#include "TuringMachine.h"

int main(){

  // States
  std::set<char> Q; Q.insert('A'); Q.insert('B'); Q.insert('C'); Q.insert('D');
  Q.insert('E'); Q.insert('F'); Q.insert('G');
  // Input symbols
  std::set<char> S; S.insert('0'); S.insert('1');
  // Tape symbols
  std::set<char> G(Q); G.insert('X'); G.insert ('_');
  // Accept states
  std::set<char> F; F.insert('G');
  // Start state
  char q0 = 'A';
  // Blank symbol
  char b = '_';
  // Delta function infile
  std::string deltaFile = "palindrome.txt";

  TuringMachine M (Q,S,G,F,q0,b,deltaFile);
  //M.check();
  M.run(true, "100100");

}
