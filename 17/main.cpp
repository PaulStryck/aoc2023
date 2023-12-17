#include <fstream>
#include <chrono>
#include <cmath>
#include <functional>
#include <queue>
#include <list>
#include <iostream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>


// use uint16_t to allow concatenation of 2 Points + additional info in a uint64_t for unordered_map hashes.
using Point = uint16_t;
using Pos = std::pair<Point, Point>;
using heat_t = uint16_t;

/*
 * Row Major Matrix
 * A[r][c] = A[r*dim.n + c]
 */
template<typename T> struct HeatMap {
    struct {
        uint16_t m;  // Rows
        uint16_t n;  // Columns
    } dim;
    std::vector<T> data;
};


enum Dir { N = 0, E, S, W };
enum Axis {NS = 0, WE};

/*
 * Each vertex in the graph is uniquely identified by
 * the position its recent travelling axis.
 */
struct Vertex {
    Pos p{};
    Axis a{};

    auto operator<=>(const Vertex&) const = default;
};

template <>
struct std::hash<Vertex> {
  uint64_t operator()(const Vertex& v) const
  {
    uint64_t h{};
    h |= static_cast<uint64_t>(v.p.first);
    h |= static_cast<uint64_t>(v.p.second) << 16;
    h |= static_cast<uint64_t>(v.a) << 32;
    return h;
  }
};

/*
 * Compute all neighbors of a Vertex
 */
template<uint16_t MIN_STEP=1, uint16_t MAX_STEP=3, typename T>
[[nodiscard]] std::vector<std::pair<Vertex, heat_t>> neighbors(const HeatMap<T>& g, const Vertex& v){
    std::vector<std::pair<Vertex, heat_t>> _res{};

    auto& [r, c] = v.p;

    std::array<Dir, 2> dirs{};
    if(v.a == Axis::NS)
        dirs = {Dir::W, Dir::E};
    else if(v.a == Axis::WE)
        dirs = {Dir::N, Dir::S};

    for(Dir d: dirs){
        heat_t accHeatLoss{0};
        for(uint16_t i = 1; i <= MAX_STEP; ++i){
            if(Dir::N == d){
                //if this step would lead out of bounds, no need to check additional steps in the same direction
                if(r < i) break;
                Vertex nextV{{r-i, c}, Axis::NS};
                accHeatLoss += g.data[nextV.p.first * g.dim.n + nextV.p.second];

                // not a valid neighbor if MIN_STEP not yet reached. But we need to account for the heatloss
                if(i < MIN_STEP) continue;
                _res.emplace_back(std::make_pair(nextV, accHeatLoss));
            } else if(Dir::E == d){
                if(c+i > g.dim.n-1) break;
                Vertex nextV{{r, c+i}, Axis::WE};
                accHeatLoss += g.data[nextV.p.first * g.dim.n + nextV.p.second];
                if(i < MIN_STEP) continue;
                _res.emplace_back(std::make_pair(nextV, accHeatLoss));
            } else if(Dir::S == d){
                if(r+i > g.dim.m-1) break;
                Vertex nextV{{r+i, c}, Axis::NS};
                accHeatLoss += g.data[nextV.p.first * g.dim.n + nextV.p.second];
                if(i < MIN_STEP) continue;
                _res.emplace_back(std::make_pair(nextV, accHeatLoss));
            } else if(Dir::W == d){
                if(c < i) break;
                Vertex nextV{{r, c-i}, Axis::WE};
                accHeatLoss += g.data[nextV.p.first * g.dim.n + nextV.p.second];
                if(i < MIN_STEP) continue;
                _res.emplace_back(std::make_pair(nextV, accHeatLoss));
            }
        }
    }

    return _res;
}

/*
 * Store vertex + fScore as State for A* Algorithm.
 * fScore will be used in the prio queue.
 */
struct State {
    Vertex v{};
    heat_t fScore{};  // lower bound to heat loss when going through v
};

void print_path(const std::unordered_map<Vertex, Vertex>& cameFrom, std::unordered_map<Vertex, heat_t> gScore, Vertex current){
    std::vector<Vertex> total_path{current};
    while(cameFrom.find(current) != cameFrom.end()){
        current = cameFrom.at(current);
        total_path.emplace_back(current);
    }

    for(auto it = total_path.crbegin(); it != total_path.crend(); ++it){
        std::cout << "[" << it->p.first << ", " << it->p.second << "]: ";
        std::cout << gScore.at(*it) << std::endl;
    }
}


template<uint8_t MIN_STEP = 1, uint8_t MAX_STEP = 3, typename T>
heat_t a_star(const HeatMap<T>& g, Pos start, std::function<bool (Vertex)> end, std::function<heat_t (Vertex)> h){
    // The set of discovered nodes that may need to be (re-)expanded.
    // Priority Queue prioritizing lower bound of approx heat loss
    auto cmp = [](const State& a, const State& b) { return a.fScore > b.fScore; };
    std::priority_queue<State, std::vector<State>, decltype(cmp)> openSet{};

    // Lookup of the predecessor to a given Vertex on the shortest path to this vertex
    std::unordered_map<Vertex, Vertex> cameFrom{};

    // Total heat loss on the shortest Path from start to Vertex
    std::unordered_map<Vertex, heat_t> gScore{};

    // We have a starting position. This expands into a starting Node for all possible travelling axes
    for(auto axis: {Axis::NS, Axis::WE}){
        Vertex _v = Vertex{start, axis};
        gScore[_v] = 0;
        openSet.emplace(State{_v, h(_v)});
    }

    while(!openSet.empty()){
        const State current = openSet.top();
        openSet.pop();

        const Vertex& currV = current.v;

        if(end(currV)){
            return gScore[currV];
        }

        // Given the current vertex compute all implicitly given admissable neighbors
        for(std::pair<Vertex, heat_t> neighbor: neighbors<MIN_STEP, MAX_STEP>(g, currV)){
            auto& [nV, nHeat] = neighbor;
            heat_t tentative_gScore = gScore[currV] + nHeat; // heat
            heat_t gScoreVal = gScore.contains(nV)? gScore[nV] : UINT16_MAX;

            if(tentative_gScore < gScoreVal){
                cameFrom[nV] = currV;
                gScore[nV] = tentative_gScore;
                heat_t fScore = tentative_gScore + h(nV);
                openSet.emplace(State{nV, fScore});
            }
        }
    }

    return UINT16_MAX;
}

int main(int argc, char* argv[]) {
    if (2 != argc)
        return EXIT_FAILURE;

    std::ifstream infile(argv[1]);
    std::string sLineBuf{};

    HeatMap<heat_t> g{};

    while(getline(infile, sLineBuf)){
        g.dim.n = sLineBuf.size();
        std::vector<heat_t> _heats{};
        for(char c: sLineBuf){
            g.data.push_back(static_cast<heat_t>(c - '0'));
        }
    }
    g.dim.m = g.data.size() / g.dim.n;

    {  // Part 1
        Pos start{0, 0};
        Pos end{g.dim.m-1, g.dim.n-1};

        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        heat_t p = a_star<1, 3>(
            g, start, [&end](Vertex v){return (v.p == end);},
            [&end](Vertex v){
                // Manhatten Distance from v to the end as a lower bound
                uint8_t diffR = v.p.first > end.first ? v.p.first - end.first : end.first - v.p.first;
                uint8_t diffC = v.p.second > end.second ? v.p.second - end.second : end.second - v.p.second;
                return diffR + diffC;
            }
        );
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

        std::cout << "Prob 1: " << p << "; ";
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "[µs]" << std::endl;
    }

    {  // Part 2
        Pos start{0, 0};
        Pos end{g.dim.m-1, g.dim.n-1};

        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        heat_t p = a_star<4, 10>(
            g, start, [&end](Vertex v){return (v.p == end);},
            [&end](Vertex v){
                // Manhatten Distance from v to the end as a lower bound
                uint8_t diffR = v.p.first > end.first ? v.p.first - end.first : end.first - v.p.first;
                uint8_t diffC = v.p.second > end.second ? v.p.second - end.second : end.second - v.p.second;
                return diffR + diffC;
            }
        );

        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

        std::cout << "Prob 2: " << p << "; ";
        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << "[µs]" << std::endl;
    }

    return EXIT_SUCCESS;
}
