#include <fstream>
#include <queue>
#include <list>
#include <iostream>
#include <algorithm>
#include <map>
#include <set>

using Pos = std::pair<size_t, size_t>;
enum Dir {
    N = 0,
    E,
    S,
    W
};
using Beam = std::pair<Pos, Dir>;

/*
 * move the beam b on contraption.
 * Contraption is only used for bounds checking
 * beam at t+1 is stored in out.
 *
 * @return true if the movement was successfull. False, if the beam left the contraption
 */
bool move(Beam b, std::vector<std::string>& contraption, Beam& out){
    auto& [p, d] = b;
    auto [r, c] = p;

    if(d == Dir::N && r > 0){
        out = {{r-1, c}, N};
        return true;
    }else if( d == Dir::E && c < contraption[r].length()-1){
        out = {{r, c+1}, E};
        return true;
    }else if(d == Dir::S && r < contraption.size() -1){
        out = {{r+1, c}, Dir::S};
        return true;
    }else if(d == Dir::W && c > 0){
        out = {{r, c-1}, Dir::W};
        return true;
    }
    return false;
}

size_t energize(std::vector<std::string>& contraption, Beam start){
    std::queue<Beam> beams{};
    std::set<Beam> cache{};
    std::set<Pos> energized{};

    beams.push(start);

    for (; !beams.empty(); beams.pop()){
        Beam b = beams.front();
        auto& [p, d] = b;
        auto& [r, c] = p;
        char ch = contraption[r][c];

        if(auto it = cache.find(b); it != cache.end()){
            continue;
        }
        cache.insert(b);
        energized.insert(p);


        Beam newPos{};
        if(ch == '.'){
            // just move
            if(move(b, contraption, newPos))
                beams.push(newPos);
        }else if(ch == '-'){
            if(d == Dir::W || d == Dir::E){
                // just move
                if(move(b, contraption, newPos))
                    beams.push(newPos);
            }else{
                // split
                if(move({p, Dir::E}, contraption, newPos))
                    beams.push(newPos);
                if(move({p, Dir::W}, contraption, newPos))
                    beams.push(newPos);
            }
        }else if(ch == '|'){
            if(d == Dir::N || d == Dir::S){
                // Just move
                if(move(b, contraption, newPos))
                    beams.push(newPos);
            }else{
                // split
                if(move({p, Dir::N}, contraption, newPos))
                    beams.push(newPos);
                if(move({p, Dir::S}, contraption, newPos))
                    beams.push(newPos);
            }
        }else if(ch == '/'){
            // reflect
            Dir _d{};
            switch(d){
                case Dir::N:
                    _d = Dir::E;
                    break;
                case Dir::E:
                    _d = Dir::N;
                    break;
                case Dir::S:
                    _d = Dir::W;
                    break;
                case Dir::W:
                    _d = Dir::S;
                    break;
            }
            if(move({p, _d}, contraption, newPos))
                beams.push(newPos);
        }else if(ch == '\\'){
            // reflect
            Dir _d{};
            switch(d){
                case Dir::N:
                    _d = Dir::W;
                    break;
                case Dir::E:
                    _d = Dir::S;
                    break;
                case Dir::S:
                    _d = Dir::E;
                    break;
                case Dir::W:
                    _d = Dir::N;
                    break;
            }
            if(move({p, _d}, contraption, newPos))
                beams.push(newPos);
        }
    }

    return energized.size();
}

int main(int argc, char* argv[]) {
    if (2 != argc)
        return EXIT_FAILURE;

    {  // Part 1
        std::ifstream infile(argv[1]);
        std::string sLineBuf{};

        std::vector<std::string> contraption{};

        while(getline(infile, sLineBuf)){
            contraption.push_back(sLineBuf);
        }

        size_t energy = energize(contraption, {{0,0}, Dir::E});
        std::cout << "Prob 1: " << energy << std::endl;
    }

    {  // Part 2
        std::ifstream infile(argv[1]);
        std::string sLineBuf{};

        std::vector<std::string> contraption{};

        while(getline(infile, sLineBuf)){
            contraption.push_back(sLineBuf);
        }

        size_t max = 0;

        for(size_t r = 0; r < contraption.size(); ++r){
            max = std::max(max, energize(contraption, {{r, 0}, Dir::E}));
            max = std::max(max, energize(contraption, {{r, contraption.size()-1}, Dir::W}));
        }

        for(size_t c = 0; c < contraption.front().size(); ++c){
            max = std::max(max, energize(contraption, {{0, c}, Dir::S}));
            max = std::max(max, energize(contraption, {{contraption.front().size()-1, c}, Dir::N}));
        }

        std::cout << "Prob 2: " << max << std::endl;
    }


    return EXIT_SUCCESS;
}
