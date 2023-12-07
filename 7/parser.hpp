#ifndef parser_HEADER
#define parser_HEADER

#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <regex>
#include <string>
#include <map>
#include <ranges>
#include <cassert>

#include "datastructures.hpp"

/*
 * parse a character into a playing card
 *
 * T \in {prob_1::Cards, prob_2::Cards} as the defined order is different
 */
template<typename T>
inline T parseCard(const char &c){
  switch(c){
    case '2':
      return T::N2;
    case '3':
      return T::N3;
    case '4':
      return T::N4;
    case '5':
      return T::N5;
    case '6':
      return T::N6;
    case '7':
      return T::N7;
    case '8':
      return T::N8;
    case '9':
      return T::N9;
    case 'T':
      return T::T;
    case 'J':
      return T::J;
    case 'Q':
      return T::Q;
    case 'K':
      return T::K;
    case 'A':
      return T::A;
  }

  // default to lowest
  return T::N2;
}

namespace prob_1 {
  /*
   * Evaluate a hand of cards by checking from highest to lowest type
   */
  HandType checkHand(const Hand<prob_1::Cards> &h){
    std::map<prob_1::Cards, unsigned int> cardCounts;

    for(prob_1::Cards c: h.cards)
      cardCounts[c] += 1;

    // Five of a kind
    if(std::any_of(
          cardCounts.begin(), cardCounts.end(),
          [](std::pair<Cards, unsigned int> p){return p.second == 5;}
    ))
      return FiveOfAKind;

    // Four of a kind
    if(std::any_of(cardCounts.begin(), cardCounts.end(), [](std::pair<Cards, unsigned int> p){return p.second == 4;}))
      return FourOfAKind;

    // Full House
    auto findThreeOfAKind = std::find_if(
        cardCounts.begin(), cardCounts.end(),
        [](const std::pair<Cards, unsigned int> &p){
          return p.second == 3;
        }
    );
    if(findThreeOfAKind != cardCounts.end() && std::any_of(
          cardCounts.begin(), cardCounts.end(),
          [&findThreeOfAKind](std::pair<Cards, unsigned int> p){
            return p.first != findThreeOfAKind->first && p.second == 2;
          }
      ))
        return FullHouse;

    // Three of a Kind
    if(std::any_of(cardCounts.begin(), cardCounts.end(), [](std::pair<Cards, unsigned int> p){return p.second == 3;}))
      return ThreeOfAKind;

    // count pairs
    unsigned int cntPairs = std::reduce(
        cardCounts.begin(), cardCounts.end(),
        (unsigned int)0, [](unsigned int prs, std::pair<Cards, unsigned int> p){
          if(p.second == 2)
            return prs +1;
          return prs;
        }
    );

    // Two Pair
    if(cntPairs >= 2)
      return TwoPair;

    // One Pair
    if(cntPairs == 1)
      return OnePair;

    return HighCard;
  }
} // namespace prob_1

namespace prob_2 {
HandType checkHand(const Hand<prob_2::Cards> &h){
  std::map<prob_2::Cards, unsigned int> cardCounts;

  for(prob_2::Cards c: h.cards)
    cardCounts[c] += 1;

  // Five of a kind
  if(std::any_of(
        cardCounts.begin(), cardCounts.end(),
        [&cardCounts](std::pair<Cards, unsigned int> p){
          if(p.second == 5)
            return true;
          if(p.first != J && p.second + cardCounts[J] == 5)
            return true;

          return false;
        }
  ))
    return FiveOfAKind;

  // Four of a kind
  if(std::any_of(
        cardCounts.begin(), cardCounts.end(),
        [&cardCounts](std::pair<Cards, unsigned int> p){
          if(p.second == 4)
            return true;
          if(p.first != J && p.second + cardCounts[J] == 4)
            return true;

          return false;
        }
  ))
    return FourOfAKind;

  // Full House
  {
    std::map<prob_2::Cards, unsigned int> _cardCounts{cardCounts};
    auto findThreeOfAKind = std::find_if(
        _cardCounts.begin(), _cardCounts.end(),
        [&_cardCounts](const std::pair<Cards, unsigned int> &p){
            if(p.second == 3)
              return true;
            if(p.first != J && p.second + _cardCounts[J] == 3){
              _cardCounts[J] -= 3 - p.second;
              return true;
            }

            return false;
        }
    );
    if(findThreeOfAKind != _cardCounts.end() && std::any_of(
          _cardCounts.begin(), _cardCounts.end(),
          [&](std::pair<Cards, unsigned int> p){
            if(p.first != findThreeOfAKind->first && p.second + _cardCounts[J] >= 2)
                return true;

            return false;
          }
      ))
        return FullHouse;
  }

  // Three of a Kind
  if(std::any_of(
        cardCounts.begin(), cardCounts.end(),
        [&cardCounts](std::pair<Cards, unsigned int> p){
          if(p.second == 3)
            return true;
          if(p.first != J && p.second + cardCounts[J] >= 3)
            return true;

          return false;
        }
  ))
    return ThreeOfAKind;

  // count pairs
  unsigned int cntPairs;
  {
    std::map<prob_2::Cards, unsigned int> _cardCounts{cardCounts};
    cntPairs = std::reduce(
        _cardCounts.begin(), _cardCounts.end(),
        (unsigned int)0, [&_cardCounts](unsigned int prs, std::pair<Cards, unsigned int> p){
          if(p.first != J && p.second + _cardCounts[J] >= 2){
            _cardCounts[J] -= 2 - p.second;
            return prs + 1;
          }
          return prs;
        }
    );
  }

  // Two Pair
  if(cntPairs >= 2)
    return TwoPair;

  // One Pair
  if(cntPairs == 1)
    return OnePair;

  return HighCard;
}
} // namespace prob_2


template<typename T>
[[nodiscard]] Hand<T> parseHand(std::string_view s) {
  Hand<T> h;

  // split into cards and bid, assert we really have both
  auto splits = std::views::split(s, ' ');
  assert(std::distance(splits.begin(), splits.end()) >= 2);

  // get the cards part of the string
  auto sCards = std::string_view{*splits.begin()};

  // assign Hand Cards
  size_t i = 0;
  for(auto c = sCards.begin(); c != sCards.end() && i < 5; ++c, ++i){
    h.cards[i] = parseCard<T>(*c);
  }

  // assign Bid, requires a string copy of the bid as stoul does not work on views
  h.bid = std::stoul ( std::string( std::string_view(*(std::next(splits.begin()))) ) );

  // get the hands type
  h.h = checkHand(h);

  return h;
}

#endif
