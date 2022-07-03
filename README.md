# WordHuntSolver
Solver for Word Hunt written in C++. 

Type in each line of the game board and the program will solve all possible words avalible and sort them from longest to shortest. Supports rectangular and square game boards. 

![](https://github.com/timroty/WordHuntSolver/blob/main/ezgif-1-5f7566c0c6.gif)

### Compile and Run
```g++ -o main main.cpp ```  
``` ./main ```

There should not be any external dependencies required.

### About

The program uses a file of valid scabble words to create a trie tree. From there, a DFS like solver is used to add neighbors of tiles that have the possibility of leading to valid words. Within each word chain, the visited words are noted so the solver does not reuse the same letter position in a word. However, the visited positions are not stored globally since the same position can be used in  multiple words. 
