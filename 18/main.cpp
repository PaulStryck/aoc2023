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

/*
 * Basic 2d point supporting addition and scalar multiplication
 */
template<typename T>
struct Pos {
    T x;
    T y;
};
template<typename T> Pos<T> operator*(T a, const Pos<T>& v) { return {v.x*a, v.y*a}; }
template<typename T> Pos<T> operator*(const Pos<T>& v, T a) { return {v.x*a, v.y*a}; }
template<typename T> Pos<T> operator+(const Pos<T>& a, const Pos<T>& b) { return {a.x+b.x, a.y+b.y}; }

enum Dir {R = 0, D = 1, L = 2, U = 3};
std::ostream& operator<<(std::ostream& out, const Dir value){
    static const auto strings = []{
        std::map<Dir, std::string_view> result;
    #define INSERT_ELEMENT(p) result.emplace(p, #p);
        INSERT_ELEMENT(Dir::R);
        INSERT_ELEMENT(Dir::D);
        INSERT_ELEMENT(Dir::L);
        INSERT_ELEMENT(Dir::U);
    #undef INSERT_ELEMENT
        return result;
    };

    return out << strings()[value];
}
constexpr auto dirs = std::array<Pos<long>, 4>{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};

Dir to_dir(char c){
    if(c == 'U' || c == '3')
        return Dir::U;

    if(c == 'R' || c == '0')
        return Dir::R;

    if(c == 'D' || c == '1')
        return Dir::D;

    if(c == 'L' || c == '2')
        return Dir::L;

    throw;
}



int main(int argc, char* argv[]) {
    if (2 != argc)
        return EXIT_FAILURE;

    { // Part 1
        std::ifstream infile(argv[1]);
        std::string sLineBuf{};

        Pos<long> curr{0,0};
        int area2{0};
        int boundary{0};
        while(getline(infile, sLineBuf)){
            std::istringstream iss{sLineBuf};
            long s;

            Dir d = to_dir(sLineBuf[0]);  // read direction as single char
            iss.ignore();  // and ignore it from the stream
            iss >> s;  // parse following number as integer

            Pos<long> next = curr + (dirs[d] * s);

            // compute area with Trapezoidal rule
            area2 += (curr.y + next.y) * (curr.x - next.x);

            // add to current boundary length
            boundary += s;

            curr = next;
        }

        // compute result using Pick's theorem + boundary
        std::cout << (std::abs(area2) + boundary) / 2 + 1 << std::endl;
    }

    { // part 2
        std::ifstream infile(argv[1]);
        std::string sLineBuf{};

        Pos<long> curr{0,0};
        long area2{0};
        long boundary{0};
        while(getline(infile, sLineBuf)){
            std::istringstream iss{sLineBuf};

            iss.ignore(); // ignore initial direction char
            long s;
            std::string color{};
            iss >> s >> color;  // read color

            Dir d = to_dir(color[7]);  // get direction from last char of color
            s = std::stol(color.substr(2,5), nullptr, 16);  // convert hex code to line segment length

            Pos<long> next = curr + (dirs[d] * s);

            area2 += (curr.y + next.y) * (curr.x - next.x);
            boundary += s;

            curr = next;
        }

        std::cout << (std::abs(area2) + boundary) / 2 + 1 << std::endl;
    }

    return EXIT_SUCCESS;
}
