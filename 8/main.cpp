#include <iostream>
#include <fstream>
#include <set>
#include <numeric>
#include "datastructures.hpp"
#include "parser.hpp"


using namespace std;

/*
 * Traverse the graph g along the path given in sTokens.
 * Counting the number of steps to get from start to a position which satisfies p.
 */
unsigned long countMinSteps(const graph_t &g, string_view sTokens, vertex_t start, function <bool (const vertex_t &v)> p){
  vertex_t nextNode = start;
  auto it = sTokens.begin();
  unsigned long steps = 0;  // count the number of steps
  while(true){
    if(p(nextNode))
      break;

    if(*it == 'L')
      nextNode = g.at(nextNode).first;
    else
      nextNode = g.at(nextNode).second;

    ++steps;

    if(++it == sTokens.end())
      it = sTokens.begin();
  }

  return steps;
}

int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;
  graph_t graph;

  ifstream infile(argv[1]);

  string sLineBuf, sTokens;

  // get the instruction line
  getline(infile, sTokens);
  getline(infile, sLineBuf);  // ignore empty line

  // build the graph
  while(getline(infile, sLineBuf)){
    pair<vertex_t, pair<vertex_t, vertex_t>> v = parseNode(sLineBuf);
    graph[v.first] = v.second;
  }

  {  // Problem 1
     // Count minimum steps required to get from AAA to ZZZ
    vertex_t stopVertex = {'Z', 'Z', 'Z'};
    unsigned long steps = countMinSteps(graph, sTokens, {'A', 'A', 'A'}, [&stopVertex](const vertex_t &v){
      return v == stopVertex;
    });
    cout << "Problem 1: " << steps << endl;
  }

  { // problem 2
    // Solution is given as the least common multiple
    // of the required steps for all starting positions.
    // I.e., lcm(minSteps('AAA'), minSteps('ABA'), minSteps('BBA'))
    // using lcm(a, b, c) == lcm(a, lcm(b, c)) a reduce can be used.
    unsigned long i = reduce(
        graph.begin(), graph.end(),
        1UL,
        [=](unsigned long x, pair<vertex_t, pair<vertex_t, vertex_t>> v){
          if(v.first.name[2] != 'A')
            return x;  //nothing to do if it is not a valid starting position

          // count steps from ..A to ..Z
          unsigned long steps = countMinSteps(graph, sTokens, v.first, [](const vertex_t &v){
            return v.name[2] == 'Z';
          });
          return lcm(x, steps);
        }
    );

    cout << "Problem 2: " << i << endl;
  }

  return EXIT_SUCCESS;
}
