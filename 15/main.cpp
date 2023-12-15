#include <fstream>
#include <list>
#include <iostream>
#include <map>
#include <vector>

using Label = std::string;
using Lens = std::pair<Label, unsigned int>;

unsigned int hash(std::string_view s){
    unsigned int val = 0;

    for(char c: s){
        if(c == '\n') continue;

        val += static_cast<unsigned int>(c);
        val *= 17;
        val %= 256;
    }

    return val;
}

int main(int argc, char* argv[]) {
    if (2 != argc)
        return EXIT_FAILURE;

    {  // Part 1
        std::ifstream infile(argv[1]);
        std::string sLineBuf{};

        unsigned int sum = 0;
        while(getline(infile, sLineBuf, ',')){
            sum += hash(sLineBuf);
        }

        std::cout << "prob 1: " << sum << std::endl;
    }

    {  // Part 2
        std::array<std::list<Lens>, 256> boxes;

        std::ifstream infile(argv[1]);
        std::string sLineBuf{};
        while(getline(infile, sLineBuf, ',')){
            if(sLineBuf.back() == '\n'){
                sLineBuf = sLineBuf.substr(0, sLineBuf.length() -1);
            }

            if(sLineBuf.back() == '-'){
                Label label = sLineBuf.substr(0, sLineBuf.length()-1);
                unsigned int box = hash(label);
                auto it = std::find_if(boxes[box].begin(), boxes[box].end(), [&label](Lens _l){
                    if(_l.first == label)
                        return true;
                    return false;
                });

                if(it != boxes[box].end())
                    boxes[box].remove(*it);
            } else {
                Lens l{sLineBuf.substr(0, sLineBuf.length() -2), sLineBuf.back() - '0'};
                unsigned int box = hash(l.first);
                auto it = std::find_if(boxes[box].begin(), boxes[box].end(), [&l](Lens _l){
                    if(_l.first == l.first)
                        return true;
                    return false;
                });
                if(it != boxes[box].end()){
                    it->second = l.second;
                }else{
                    boxes[box].push_back(l);
                }
            }

        }

        unsigned int sum = 0;
        for(size_t b = 0; b < boxes.size(); ++b){
            size_t i = 0;
            for(auto s: boxes[b]){
                ++i;
                sum += (b+1) * i * s.second;
            }
        }

        std::cout << "prob 2: " << sum << std::endl;
    }

    return EXIT_SUCCESS;
}
