#ifndef datastructures_HEADER
#define datastructures_HEADER

#include <vector>
#include <map>

enum Direction {
  NORTH = 1,
  EAST,
  SOUTH,
  WEST
};

typedef std::pair<int, int> coord_t;
struct connectivity_t {
  bool north;
  bool east;
  bool south;
  bool west;
};
typedef std::map<coord_t, connectivity_t> graph_t;

/*
 * Very basic, non thread-safe cyclic buffer.
 */
template<typename T, std::size_t buffer_size = 3>
class Ringbuffer {
public:
  std::atomic<size_t> head;

  std::array<T, buffer_size> data_buff;

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

#endif
