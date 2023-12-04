#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>

#include <functional>
#include <iterator>
#include <algorithm>


using namespace std;

size_t parseNumbersUntil(string_view s, char delim, std::function <void (int num)> fn) {
  // string buffer beeing filled with digits
  std::string f = "";

  // are we currently parsing a number
  bool parsing = false;

  // tracking current index of a character
  size_t i = 0;
  for(auto it = s.begin(); it != s.end() && *it != delim; ++it, ++i){
    // loop the entire string
    if(isdigit(*it)){
      // append any digit to the buffer
      f += *it;

      if(!parsing){
        // we've hit the first digit
        parsing = true;
      }
    }

    if(!f.empty() && (!isdigit(*it) || (std::next(it) == s.end() ) || (*std::next(it) == delim) ) ){
      // we've hit the end of a number
      // either because digits were parsed before,
      //   but current char is not a digit
      // or digits were parsed before and we've hit
      //   the end of the string

      int f_ = atoi(f.c_str());

      f = "";  // reset string buffer
      parsing = false;

      fn(f_);
    }
  }
  return i;
}

int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

  ifstream infile(argv[1]);

  set<int> winningNumbers;
  set<int> myNumbers;
  map<int, int> scratchCards;

  string l;
  int sum_1 = 0;
  int sum_2 = 0;

  while(getline(infile, l)){
    winningNumbers.clear();
    myNumbers.clear();

    // size_t offset = 8;
    size_t offset = 0;
    int iCard = 0;

    // Parse first part: "Card xxx: " and get the integer xxx.
    offset = parseNumbersUntil(l.substr(offset), ':', [&iCard](int num){
        iCard = num;
    });
    scratchCards.try_emplace(iCard, 0);
    ++scratchCards[iCard];

    // Parse the winning numbers
    offset += parseNumbersUntil(l.substr(offset), '|', [&](int num){
        winningNumbers.insert(num);
    });

    // Parse my numbers
    parseNumbersUntil(l.substr(offset), '\0', [&](int num){
        myNumbers.insert(num);
    });

    // Points are scored for the interseciont between winningNumbers and myNumbers
    set<int> intersect;
    set_intersection(
        myNumbers.begin(), myNumbers.end(),
        winningNumbers.begin(), winningNumbers.end(),
        std::inserter(intersect, intersect.begin())
    );

    if(intersect.size() > 0)
      sum_1 += 1 << (intersect.size() -1);

    for(size_t i = 0; i < intersect.size(); ++i){
        int jCard = iCard + static_cast<int>(i) + 1;
        scratchCards.try_emplace(jCard, 0);
        scratchCards[jCard] += scratchCards[iCard];
    }
  }


  // For problem 2
  sum_2 = reduce(
      scratchCards.cbegin(), scratchCards.cend(),
      0, [](int a, const pair<const int, int> b){return a + b.second;}
  );

  cout << sum_1 << endl;
  cout << sum_2 << endl;


  return EXIT_SUCCESS;
}
