#include <iostream>
#include <fstream>
#include <set>
#include <numeric>
#include "datastructures.hpp"
#include "parser.hpp"


using namespace std;

int binomialCoeff(int n, int k)
{
    int res = 1;

    // Since C(n, k) = C(n, n-k)
    if (k > n - k)
        k = n - k;

    // Calculate value of
    // [n * (n-1) *---* (n-k+1)] / [k * (k-1) *----* 1]
    for (int i = 0; i < k; ++i) {
        res *= (n - i);
        res /= (i + 1);
    }

    return res;
}

int predict(const oasis_t &o){
  int sum = 0;
  for(int n = 0; n < (int)o.size(); ++n){
    int k = 0;
    // sum_(k=0)^n binomial(n, k) (-1)^k x_{-k}, where -k is the kth element from the back
    // this computes the last entry of the nth row within the triangle in closed form
    for(auto it = o.rbegin(); it != o.rend() && k <= n; ++it, ++k){
      int s = k % 2 == 0 ? 1 : -1;  // (-1)^k
      int b = binomialCoeff(n, k);
      sum += s * b * (*it);
    }
  }
  return sum;
}

int main(int argc, char* argv[]) {
  if (2 != argc)
    return EXIT_FAILURE;

  ifstream infile(argv[1]);

  string sLineBuf;

  int sum_1 = 0;
  int sum_2 = 0;
  while(getline(infile, sLineBuf)){
    oasis_t o = parseOasisReading(sLineBuf);

    sum_1 += predict(o);

    reverse(o.begin(), o.end());
    sum_2 += predict(o);
  }

  cout << sum_1 << endl;
  cout << sum_2 << endl;

  return 0;
}
