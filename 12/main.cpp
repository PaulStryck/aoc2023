#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>


using Group = std::vector<unsigned int>;

template <size_t FOLD = 1>
static size_t count(
    const std::string& p,
    const Group& g,
    size_t pos,
    size_t k,
    std::shared_ptr<std::map<std::pair<size_t, size_t>, size_t> > cache)
{
    if (const auto it = cache->find({pos, k}); it != cache->end()) {
        return it->second;
    }
    if (pos == p.size() && k == FOLD * g.size()) {
        return 1;
    }
    if (pos >= p.size()) {
        return 0;
    }
    if ('#' == p[pos]) {
        if (k >= FOLD * g.size() || pos + g[k % g.size()] >= p.size()) {
            return 0;
        }
        for (size_t c = 1; c < g[k % g.size()]; ++c) {
            if ('.' == p[pos + c]) {
                return 0;
            }
        }
        pos += g[k % g.size()];
        k++;
        if ('#' == p[pos]) {
            return 0;
        }
        const auto s = count<FOLD>(p, g, pos + 1, k, cache);
        cache->insert({{pos + 1, k}, s});
        return s;
    }
    if ('.' == p[pos]) {
        return count<FOLD>(p, g, pos + 1, k, cache);
    }
    const std::string p1 = p.substr(0, pos) + '#' + p.substr(pos + 1, p.size() - pos - 1);
    const std::string p2 = p.substr(0, pos) + '.' + p.substr(pos + 1, p.size() - pos - 1);
    return count<FOLD>(p1, g, pos, k, cache) + count<FOLD>(p2, g, pos, k, cache);
}

static inline size_t count1(const std::string& p, const Group& g)
{
    return count<1>(p, g, 0, 0, std::make_shared<std::map<std::pair<size_t, size_t>, size_t> >());
}

static inline size_t count5(const std::string& p, const Group& g)
{
    return count<5>(p, g, 0, 0, std::make_shared<std::map<std::pair<size_t, size_t>, size_t> >());
}

int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

    {  // Part 1
        std::ifstream infile(argv[1]);
        std::string sLineBuf;
        size_t sum{};

        while(getline(infile, sLineBuf)){
            unsigned int x;
            std::string p;
            std::istringstream iss{sLineBuf};
            iss >> p;
            p += '.';
            Group g;
            while (iss >> x) {
                g.push_back(x);
                iss.ignore(1);
            }

            sum += count1(p, g);
        }
        std::cout << sum << std::endl;
    }
    {  // Part 2
        std::ifstream infile(argv[1]);
        std::string sLineBuf;
        size_t sum{};
        while(getline(infile, sLineBuf)){
            unsigned int x;
            std::string p;
            std::istringstream iss{sLineBuf};
            iss >> p;
            p = p + '?' + p + '?' + p + '?' + p + '?' + p + '.';
            Group g;
            while (iss >> x) {
                g.push_back(x);
                iss.ignore(1);
            }

            sum += count5(p, g);
        }

        std::cout << sum << std::endl;
    }

    return EXIT_SUCCESS;
}
