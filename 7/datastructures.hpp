#ifndef datastructures_HEADER
#define datastructures_HEADER

#include <compare>

namespace prob_1 {
enum Cards {
  N2=1, N3, N4, N5, N6, N7, N8, N9, T, J, Q, K, A
};
} // namespace prob_1

namespace prob_2 {
enum Cards {
  J=1, N2, N3, N4, N5, N6, N7, N8, N9, T, Q, K, A
};
} // namespace prob_2


enum HandType {
  HighCard = 1,
  OnePair,
  TwoPair,
  ThreeOfAKind,
  FullHouse,
  FourOfAKind,
  FiveOfAKind
};

template<typename T>
struct Hand {
  HandType h;
  T cards[5];
  unsigned int bid;

  auto operator<=>(const Hand&) const = default;
};
#endif
