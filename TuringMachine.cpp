#include "TuringMachine.h"

int main(){

  // // States
  // std::set<char> Q; Q.insert('A'); Q.insert('B'); Q.insert('C'); //Q.insert('D');
  // //Q.insert('E'); Q.insert('F'); Q.insert('G');
  // // Input symbols
  // std::set<char> S; S.insert('0'); S.insert('1'); S.insert('$');
  // // Tape symbols
  // std::set<char> G(Q); G.insert('X'); G.insert ('_');
  // // Accept states
  // std::set<char> F; F.insert('C');
  // // Start state
  // char q0 = 'A';
  // // Blank symbol
  // char b = '_';
  // // Delta function infile
  // std::string deltaFile = "0^n1^n0^n.txt";

  std::cout << "Input file name: ";
  std::string inputfile;
  std::cin >> inputfile;
  TuringMachine M (inputfile,'B','f');
  while (true){
    std::cout << std::endl << "Input string: ";
    std::cin >> inputfile;
    M.run(false, inputfile);
  }
}
