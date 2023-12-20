### Advent of Code 2023

I did the AoC puzzles to get back into the rhythm with c++ and STL.  
Each solution can be build using CMake, producing an executable which takes a path to the input file as first argument.
I.e., `./main input.txt`.  

Days 2, 19 and 20 use a BNF to generate a parser and perform the computations on the AST.
This requires the additional dependencies of `bnfc`, `Yacc` and `Bison`.
To generate the parsers run `bnfc --cpp -m -o grammar grammar.cf` within the respective days directory.
Then continue to build with CMake.

Day 5 requires BOOST as an addtional dependency because boosts interval_set made the solution much more elegant.
