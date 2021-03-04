This class simulates a Turing Machine. All parts of the tuple-definition have to be provided by the user, including a text file containing the transition function.  
How to write the transition function file:  
Every five space-delineated characters represents a value of d(q,x)->(p,y,D) where
  - q is the current state of the read/write head
  - x is the symbol being read from the tape in that cell
  - p is the new state for the read/write head to transition to
  - y is the new symbol to write to the tape
  - D is the direction for the head to move - either 'L' or 'R'

All symbols and states have to be chars.  
It's up to the user to make sure the transition function file is error-free, check() doesn't look at it!  
To halt and reject on a read, set p, y, and D to 'e' in the transition function. See palindrome.txt for examples.

To Do:  
  - [x] "Check" function that makes sure various parts of the function are valid  
    - b in G; q0 in Q; F subset Q; S subset G  
  - [x] Minimal constructor (run() doesn't actually use most of the data in the TM!)
  - [x] Minimaler constructor (automatic construction from delta file)
  - [x] Reject states
  - [x] Two-way infinite tape
  - [x] Comments starting with // are ignored
  - [x] Internal sets unordered
  - [x] Printed IDs have parentheses around the RW head for clarity
  - [ ] check() removes 'e' from Q, G, F
  - [ ] More transition function examples!
  - [ ] Instructions in an unordered set for faster lookup while running
