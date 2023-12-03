#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <iostream>
#include <cstdlib>
#include <atomic>
#include <functional>

/*
 * Very basic, non thread-safe cyclic buffer.
 */
template<typename T, std::size_t buffer_size = 3>
class Ringbuffer {
public:
  std::atomic<size_t> head;

  T data_buff[buffer_size];

  Ringbuffer() : head(0) {}
  ~Ringbuffer() {}

  /*
   * Insert an element. If the total number of elements
   * exceeds buffer_size, the oldest element will be discarded.
   */
  void insert(T data) {
    size_t head_tmp = head;

    head_tmp = (++head_tmp) % buffer_size;
    data_buff[head_tmp] = data;

    head = head_tmp;
  }

  /*
   * Access elements w.r.t. insertion order.
   * I.e.,
   *   [0] -> latest insertion
   *   [-1] -> before that
   *   [-2] -> before that
   *   ...
   * negative indices must be treated as elements
   * of the cyclic group Z_{buffer_size}.
   * For buffer_size = 3 this means:
   * [0] -> [0]  -> latest insertion
   * [-1] -> [2] -> before that
   * [-2] -> [1] -> before that
   */
  T& operator[](size_t index) {
    size_t head_tmp = head;
    head_tmp = (head + index) % buffer_size;

    return data_buff[head_tmp];
  }
};

void applyToNumbers(std::string_view s, const std::function <void (int, size_t, size_t)>& fn){
  // string buffer beeing filled with digits
  std::string f = "";

  // start, end position of a number within the string
  size_t rng[2] = {0, 0};

  // are we currently parsing a number
  bool parsing = false;

  // tracking current index of a character
  size_t i = 0;
  for(auto it = s.begin(); it != s.end(); ++it, ++i){
    // loop the entire string
    if(isdigit(*it)){
      // append any digit to the buffer
      f += *it;

      if(!parsing){
        // we've hit the first digit
        parsing = true;
        rng[0] = i;
      }
    }

    if(!f.empty() && (!isdigit(*it) || (std::next(it) == s.end() ) ) ){
      // we've hit the end of a number
      // either because digits were parsed before,
      //   but current char is not a digit
      // or digits were parsed before and we've hit
      //   the end of the string

      int f_ = atoi(f.c_str());

      f = "";  // reset string buffer
      parsing = false;


      rng[1] = i-1;  // track end index of number

      fn(f_, rng[0], rng[1]);
    }
  }
}

#endif
