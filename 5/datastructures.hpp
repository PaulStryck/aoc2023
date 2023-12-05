#ifndef datastructures_HEADER
#define datastructures_HEADER
#include <set>
#include <map>
#include <iostream>
#include <boost/icl/interval_set.hpp>

using namespace std;

class Function;
typedef std::map<string, std::pair<string, Function>> almanac_t;

typedef boost::icl::interval_set<unsigned int> domain_t;
typedef domain_t::interval_type interval;

class Function {
  public:

  struct FnDefInterval {
    interval domain;
    int offset;

    // Interval order is a weak order on the set of non overlapping intervals
    std::weak_ordering operator<=>(const FnDefInterval &other)const{
      if(domain.lower() < other.domain.lower())
        return std::weak_ordering::less;

      if(domain.upper() > other.domain.upper())
        return std::weak_ordering::greater;

      return std::weak_ordering::equivalent;
    };
  };

  set<FnDefInterval> f;

  Function() {}
  Function(set<FnDefInterval> _f): f(_f){}

  unsigned int operator()(unsigned int x){
    for(auto i: f){
      if(boost::icl::contains(i.domain, x)){
        if(i.offset < 0){
          return x - (unsigned int)((-1)*i.offset);
        }
        return x + (unsigned int)i.offset;
      }
    }
    return x;
  }

  domain_t operator()(domain_t x){
    domain_t image;
    for(auto i: f){
      for(auto foo : (x & i.domain)){
        image.insert(interval::closed(
          (*this)(boost::icl::first(foo)),
          (*this)(boost::icl::last(foo))
        ));
      }

      x -= i.domain;
    }

    image += x;
    return image;
  }
};


typedef string category;
typedef struct {
  unsigned int source;
  unsigned int destination;
  unsigned int rng;
} AlmanacEntry;

#endif
