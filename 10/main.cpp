#include <iostream>
#include <fstream>
#include <map>
#include <array>

#include "datastructures.hpp"


using namespace std;

/*
 * check if a symbol is connected into Direction d
 */
bool connected(char c, Direction d) {
  switch(c){
    case '|':
      if(d == NORTH || d == SOUTH)
        return true;
      break;
    case '-':
      if(d == EAST || d == WEST)
        return true;
      break;
    case 'L':
      if((d == NORTH) || (d == EAST))
        return true;
      break;
    case 'J':
      if(d == NORTH || d == WEST)
        return true;
      break;
    case '7':
      if(d == SOUTH || d == WEST)
        return true;
      break;
    case 'F':
      if(d == SOUTH || d == EAST)
        return true;
      break;
    case 'S':
      return true;
      break;
  }
  return false;
}

/*
 * given the von Neumann neighborhood of char c,
 * compute which directions are connected to c.
 */
[[nodiscard]] connectivity_t parseConn(char c, char cNorth, char cEast, char cSouth, char cWest){
  connectivity_t conn {false, false, false, false};

  if(connected(c, NORTH) && connected(cNorth, SOUTH))
    conn.north = true;

  if(connected(c, EAST) && connected(cEast, WEST))
    conn.east = true;

  if(connected(c, WEST) && connected(cWest, EAST))
    conn.west = true;

  if(connected(c, SOUTH) && connected(cSouth, NORTH))
    conn.south = true;

  return conn;
}


/*
 * perform one step into Direction d from coord_t pos.
 */
inline coord_t step(coord_t pos, Direction d){
  switch(d){
    case NORTH:
      return coord_t{pos.first, pos.second-1};
    case EAST:
      return coord_t{pos.first+1, pos.second};
    case SOUTH:
      return coord_t{pos.first, pos.second+1};
    case WEST:
      return coord_t{pos.first-1, pos.second};
  }
}

/*
 * Check into which direction to go from coord_t pos on graph_t g
 * when having entered pos from Direction origin.
 */
Direction where(const graph_t &g, coord_t pos, Direction origin){
  connectivity_t c = g.at(pos);
  if(c.north && origin != SOUTH)
    return NORTH;

  if(c.east && origin != WEST)
    return EAST;

  if(c.south && origin != NORTH)
    return SOUTH;

  if(c.west && origin != EAST)
    return WEST;

  // Nowhere to go
  return origin;
}


/*
 * parse the center element of the Ringbuffer r into the graph g.
 * assuming
 */
void put(graph_t &g, Ringbuffer<string, 3> &r, int yPos, coord_t &start){
    int xPos = 1;
    for(auto c = next(r[2].begin()); next(c) != r[2].end(); ++c){
      coord_t pos{xPos, yPos};
      char cPos = *c;
      char cNorth = r[1][xPos];
      char cEast = *next(c);
      char cSouth = r[0][xPos];
      char cWest = *prev(c);

      if(cPos == 'S')
        start = {xPos, yPos};
      g[pos] = parseConn(cPos, cNorth, cEast, cSouth, cWest);

      ++xPos;
    }
}

int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

  graph_t graph;
  coord_t start{0, 0};
  Ringbuffer<string, 3> r;

  // To parse a line of the input maze, only the 2 neighboring lines are required.
  // Using a Ringbuffer the file can be parsed into a graph while only keeping 3 lines of the file in memory.
  ifstream infile(argv[1]);
  string sLineBuff;
  getline(infile, sLineBuff);

  // guard the borders by unreachable floor elements
  // insert a dummy line of '.' NORTH and SOUTH of the maze
  // insert '.' dummy chars in each line WEST and EAST of the maze
  r.insert(std::string(sLineBuff.length()+2, '.'));
  r.insert('.' + sLineBuff + '.');

  int yPos = 0;
  while(getline(infile, sLineBuff)){
    r.insert('.' + sLineBuff + '.');

    put(graph, r, yPos, start);

    ++yPos;
  }

  r.insert(std::string(sLineBuff.length()+2, '.'));
  put(graph, r, yPos, start);

  // Operate on the Graph
  vector<coord_t> walk;
  coord_t p = start;
  Direction d = NORTH;
  int i = 0;
  do{
    walk.emplace_back(p);
    d = where(graph, p, d);
    p = step(p, d);
    ++i;
  }while(p != start);
  walk.emplace_back(p);

  int area = 0;
  for(auto it = walk.begin(); next(it) != walk.end(); ++it){
    coord_t a = *it;
    coord_t b = *next(it);
    area += (a.second + b.second) * (a.first - b.first);
  }
  area = abs(area);
  int nPts = walk.size() -1;

  cout << "Problem 1: " << i / 2 << endl;
  cout << "Problem 2: " << (area - nPts)/2 + 1 << endl;
  return 0;
}
