#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include "parser.hpp"


void moveN(Rocks &rocks){
    auto& [_r, dim] = rocks;

    // Rows in outer loop due to Row Major storage of underlying matrix.
    for (size_t r = 1; r < dim.nRows; ++r) {
        for (size_t c = 0; c < dim.nCols; ++c) {
            if (_r[dim.nCols * r + c] == RockT::RockO) {
                size_t rr = r;
                // By moving rocks line by line, more assignments are required than strictly necessary.
                // + if a rock gets moved far, the cache line gets invalidated.
                while (rr > 0 && _r[dim.nCols * (rr - 1) + c] == RockT::NoRock) {
                    _r[dim.nCols * (rr - 1) + c] = RockT::RockO;
                    _r[dim.nCols * rr + c] = RockT::NoRock;
                    --rr;
                }
            }
        }
    }
}


void moveE(Rocks& rocks){
    auto& [_r, dim] = rocks;

    for (size_t r = 0; r < dim.nRows; ++r) {
        for (size_t c = dim.nCols-1; c > 0; --c) {
            if (_r[r*dim.nCols + c - 1] == RockT::RockO) {
                size_t cc{c - 1};
                // as long as dim.nCols < chache line length, the while loop should not invalidate the cache line
                while (cc + 1 < dim.nCols && _r[r*dim.nCols + cc + 1] == RockT::NoRock) {
                    _r[r*dim.nCols + cc + 1] = RockT::RockO;
                    _r[r*dim.nCols + cc] = RockT::NoRock;
                    ++cc;
                }
            }
        }
    }
}

void moveS(Rocks &rocks){
    auto& [_r, dim] = rocks;

    for (size_t r = dim.nRows - 1; r > 0; --r) {
        for (size_t c = 0; c < dim.nCols; ++c) {
            if (_r[(r - 1) * dim.nCols + c] == RockT::RockO) {
                size_t rr{r - 1};
                while (rr + 1 < dim.nRows && _r[(rr + 1) * dim.nCols + c] == RockT::NoRock) {
                    _r[(rr + 1) * dim.nCols + c] = RockT::RockO;
                    _r[rr*dim.nCols + c] = RockT::NoRock;
                    rr++;
                }
            }
        }
    }
}

void moveW(Rocks& rocks){
    auto& [_r, dim] = rocks;

    for (size_t r = 0; r < dim.nRows; ++r) {
        for (size_t c = 1; c < dim.nCols; ++c) {
            if (_r[r*dim.nCols + c] == RockT::RockO) {
                size_t cc{c};
                while (cc > 0 && _r[r*dim.nCols + cc - 1] == RockT::NoRock) {
                    _r[r*dim.nCols + cc - 1] = RockT::RockO;
                    _r[r*dim.nCols + cc] = RockT::NoRock;
                    cc--;
                }
            }
        }
    }
}
unsigned long calcLoad(Rocks& rocks){
    auto& [_r, dim] = rocks;
    unsigned long load = 0;

    for (size_t r = 0; r < dim.nRows; ++r) {
        for (size_t c = 0; c < dim.nRows; ++c) {
            if (_r[r*dim.nCols + c] == RockT::RockO) {
                load += dim.nRows - r;
            }
        }
    }

    return load;
}

void printRocks(Rocks rocks){
    auto& [_r, dim] = rocks;

    for(size_t i = 0; i < dim.nRows; ++i){
        for(size_t j = 0; j < dim.nCols; ++j){
            auto r = _r[i * rocks.second.nCols + j];
            switch(r){
                case RockT::RockX:
                    std::cout << '#';
                    break;
                case RockT::RockO:
                    std::cout << 'O';
                    break;
                default:
                    std::cout << '.';
            };
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (2 != argc)
        return EXIT_FAILURE;

    {  // Part 1
        std::ifstream infile(argv[1]);
        std::string sLineBuf{};
        Rocks rocks{};
        auto& [_r, dim] = rocks;

        getline(infile, sLineBuf);
        dim.nCols = sLineBuf.length();
        do{
            for(auto c: sLineBuf){
                if(c == 'O')
                    _r.push_back(RockT::RockO);
                else if(c == '#')
                    _r.push_back(RockT::RockX);
                else if(c == '.')
                    _r.push_back(RockT::NoRock);
            }
        }while(getline(infile, sLineBuf));
        dim.nRows = _r.size() / dim.nCols;

        moveN(rocks);

        std::cout << "load: " << calcLoad(rocks) << std::endl;
    }

    {  // Part 2
        std::ifstream infile(argv[1]);
        std::string sLineBuf{};
        Rocks rocks{};
        auto& [_r, dim] = rocks;

        getline(infile, sLineBuf);
        dim.nCols = sLineBuf.length();
        do{
            for(auto c: sLineBuf){
                if(c == 'O')
                    _r.push_back(RockT::RockO);
                else if(c == '#')
                    _r.push_back(RockT::RockX);
                else if(c == '.')
                    _r.push_back(RockT::NoRock);
            }
        }while(getline(infile, sLineBuf));
        dim.nRows = _r.size() / dim.nCols;

        std::map<std::vector<RockT>, size_t> cache;
        size_t i = 0;

        cache.insert({rocks.first, i});
        while(i++ < 1000000000){
            moveN(rocks);
            moveW(rocks);
            moveS(rocks);
            moveE(rocks);

            auto it = cache.find(rocks.first);
            if (it != cache.end()) {
                std::cout << "cycle: " << i << std::endl;
                const auto period = i - it->second;
                i = 1000000000 - (1000000000 - i) % period;
                cache.clear();
            } else if (!cache.empty()) {
                cache.insert({rocks.first, i});
            }
        }
        std::cout << "load: " << calcLoad(rocks) << std::endl;

    }


    return EXIT_SUCCESS;
}
