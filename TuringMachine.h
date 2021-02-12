#include <set>
#include <fstream>

class TuringMachine{
private:
  std::set<char> Q;  // Set of states for the R/W head
  std::set<char> S;  // Sigma, set of input symbols; subset of G
  std::set<char> G;  // Gamma, set of tape symbols
  std::set<char> F;  // F, set of accept states; subset of Q
  char q0;      // q_0, start state of the R/W head; in Q
  char b;       // b, blank tape symbol; in G
  std::string deltaFile; // delta, name of filename to be parsed for transition function

  char delta(std::string d,char q,char x, char out);
  // returns p,y, or L/R for a machine in state q reading x

/*
  // char delta_p(char d,char q,char x);  // returns new state for machine in state q reading tape x
  // char delta_x(char d,char q,char x);  // returns y to write to tape in state q reading tape x
  // char delta_D(char d, char q, char x);// returns direction for head to move
*/
  bool is_in(char c, std::set<char> s);

public:
  // Constructor
  TuringMachine (std::set<char> Qi, std::set<char> Si, std::set<char> Gi, std::set<char> Fi,
    char q0i, char bi, std::string deltai);
  // Run the machine on an input string.
  // init - string that starts on the tape
  // step - controls whether to run it fast or step forward
  bool run(bool step, std::string init);
};
