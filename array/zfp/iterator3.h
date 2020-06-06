// random access const iterator that visits 3D array or view block by block; this class is nested within container_type
class const_iterator : public const_handle {
public:
  // typedefs for STL compatibility
  typedef typename container_type::value_type value_type;
  typedef ptrdiff_t difference_type;
  typedef typename container_type::reference reference;
  typedef typename container_type::pointer pointer;
  typedef std::random_access_iterator_tag iterator_category;

  typedef typename container_type::const_reference const_reference;
  typedef typename container_type::const_pointer const_pointer;

  // default constructor
  const_iterator() : const_handle(0, 0, 0, 0) {}

  // constructor
  explicit const_iterator(const container_type* container, size_t x, size_t y, size_t z) : const_handle(container, x, y, z) {}

  // dereference iterator
  const_reference operator*() const { return const_reference(container, x, y, z); }
  const_reference operator[](difference_type d) const { return *operator+(d); }

  // iterator arithmetic
  const_iterator operator+(difference_type d) const { const_iterator it = *this; it.advance(d); return it; }
  const_iterator operator-(difference_type d) const { return operator+(-d); }
  difference_type operator-(const const_iterator& it) const { return offset() - it.offset(); }

  // equality operators
  bool operator==(const const_iterator& it) const { return container == it.container && x == it.x && y == it.y && z == it.z; }
  bool operator!=(const const_iterator& it) const { return !operator==(it); }

  // relational operators
  bool operator<=(const const_iterator& it) const { return container == it.container && offset() <= it.offset(); }
  bool operator>=(const const_iterator& it) const { return container == it.container && offset() >= it.offset(); }
  bool operator<(const const_iterator& it) const { return container == it.container && offset() < it.offset(); }
  bool operator>(const const_iterator& it) const { return container == it.container && offset() > it.offset(); }

  // increment and decrement
  const_iterator& operator++() { increment(); return *this; }
  const_iterator& operator--() { decrement(); return *this; }
  const_iterator operator++(int) { const_iterator it = *this; increment(); return it; }
  const_iterator operator--(int) { const_iterator it = *this; decrement(); return it; }
  const_iterator operator+=(difference_type d) { advance(+d); return *this; }
  const_iterator operator-=(difference_type d) { advance(-d); return *this; }

  // local container indices of value referenced by iterator
  size_t i() const { return x - container->min_x(); }
  size_t j() const { return y - container->min_y(); }
  size_t k() const { return z - container->min_z(); }

protected:
  // sequential offset associated with index (x, y, z) plus delta d
  difference_type offset(difference_type d = 0) const
  {
    difference_type p = d;
    size_t xmin = container->min_x();
    size_t xmax = container->max_x();
    size_t ymin = container->min_y();
    size_t ymax = container->max_y();
    size_t zmin = container->min_z();
    size_t zmax = container->max_z();
    size_t nx = xmax - xmin;
    size_t ny = ymax - ymin;
    size_t nz = zmax - zmin;
    if (z == zmax)
      p += nx * ny * nz;
    else {
      size_t m = ~size_t(3);
      size_t bz = std::max(z & m, zmin); size_t sz = std::min((bz + 4) & m, zmax) - bz; p += (bz - zmin) * nx * ny;
      size_t by = std::max(y & m, ymin); size_t sy = std::min((by + 4) & m, ymax) - by; p += (by - ymin) * nx * sz;
      size_t bx = std::max(x & m, xmin); size_t sx = std::min((bx + 4) & m, xmax) - bx; p += (bx - xmin) * sy * sz;
      p += (z - bz) * sx * sy;
      p += (y - by) * sx;
      p += (x - bx);
    }
    return p;
  }

  // index (x, y, z) associated with sequential offset p
  void index(size_t& x, size_t& y, size_t& z, difference_type p) const
  {
    size_t xmin = container->min_x();
    size_t xmax = container->max_x();
    size_t ymin = container->min_y();
    size_t ymax = container->max_y();
    size_t zmin = container->min_z();
    size_t zmax = container->max_z();
    size_t nx = xmax - xmin;
    size_t ny = ymax - ymin;
    size_t nz = zmax - zmin;
    if (p == nx * ny * nz) {
      x = xmin;
      y = ymin;
      z = zmax;
    }
    else {
      size_t m = ~size_t(3);
      size_t bz = std::max((zmin + p / (nx * ny)) & m, zmin); size_t sz = std::min((bz + 4) & m, zmax) - bz; p -= (bz - zmin) * nx * ny;
      size_t by = std::max((ymin + p / (nx * sz)) & m, ymin); size_t sy = std::min((by + 4) & m, ymax) - by; p -= (by - ymin) * nx * sz;
      size_t bx = std::max((xmin + p / (sy * sz)) & m, xmin); size_t sx = std::min((bx + 4) & m, xmax) - bx; p -= (bx - xmin) * sy * sz;
      z = bz + p / (sx * sy); p -= (z - bz) * sx * sy;
      y = by + p / sx;        p -= (y - by) * sx;
      x = bx + p;             p -= (x - bx);
    }
  }

  // advance iterator by d
  void advance(difference_type d) { index(x, y, z, offset(d)); }

  // increment iterator to next element
  void increment()
  {
    size_t xmin = container->min_x();
    size_t xmax = container->max_x();
    size_t ymin = container->min_y();
    size_t ymax = container->max_y();
    size_t zmin = container->min_z();
    size_t zmax = container->max_z();
    size_t m = ~size_t(3);
    ++x;
    if (!(x & 3u) || x == xmax) {
      x = std::max((x - 1) & m, xmin);
      ++y;
      if (!(y & 3u) || y == ymax) {
        y = std::max((y - 1) & m, ymin);
        ++z;
        if (!(z & 3u) || z == zmax) {
          z = std::max((z - 1) & m, zmin);
          // done with block; advance to next
          x = (x + 4) & m;
          if (x >= xmax) {
            x = xmin;
            y = (y + 4) & m;
            if (y >= ymax) {
              y = ymin;
              z = (z + 4) & m;
              if (z >= zmax)
                z = zmax;
            }
          }
        }
      }
    }
  }

  // decrement iterator to previous element
  void decrement()
  {
    size_t xmin = container->min_x();
    size_t xmax = container->max_x();
    size_t ymin = container->min_y();
    size_t ymax = container->max_y();
    size_t zmin = container->min_z();
    size_t zmax = container->max_z();
    size_t m = ~size_t(3);
    if (z == zmax) {
      x = xmax - 1;
      y = ymax - 1;
      z = zmax - 1;
    }
    else {
      if (!(x & 3u) || x == xmin) {
        x = std::min((x + 4) & m, xmax);
        if (!(y & 3u) || y == ymin) {
          y = std::min((y + 4) & m, ymax);
          if (!(z & 3u) || z == zmin) {
            z = std::min((z + 4) & m, zmax);
            // done with block; advance to next
            x = (x - 1) & m;
            if (x <= xmin) {
              x = xmax;
              y = (y - 1) & m;
              if (y <= ymin) {
                y = ymax;
                z = (z - 1) & m;
                if (z <= zmin)
                  z = zmin;
              }
            }
          }
          --z;
        }
        --y;
      }
      --x;
    }
  }

  using const_handle::container;
  using const_handle::x;
  using const_handle::y;
  using const_handle::z;
};

// random access iterator that visits 3D array or view block by block; this class is nested within container_type
class iterator : public const_iterator {
public:
  // typedefs for STL compatibility
  typedef typename container_type::value_type value_type;
  typedef ptrdiff_t difference_type;
  typedef typename container_type::reference reference;
  typedef typename container_type::pointer pointer;
  typedef std::random_access_iterator_tag iterator_category;

  // default constructor
  iterator() : const_iterator(0, 0, 0, 0) {}

  // constructor
  explicit iterator(container_type* container, size_t x, size_t y, size_t z) : const_iterator(container, x, y, z) {}

  // dereference iterator
  reference operator*() const { return reference(container, x, y, z); }
  reference operator[](difference_type d) const { return *operator+(d); }

  // iterator arithmetic
  iterator operator+(difference_type d) const { iterator it = *this; it.advance(d); return it; }
  iterator operator-(difference_type d) const { return operator+(-d); }
  difference_type operator-(const iterator& it) const { return offset() - it.offset(); }

  // equality operators
  bool operator==(const iterator& it) const { return container == it.container && x == it.x && y == it.y && z == it.z; }
  bool operator!=(const iterator& it) const { return !operator==(it); }

  // relational operators
  bool operator<=(const iterator& it) const { return container == it.container && offset() <= it.offset(); }
  bool operator>=(const iterator& it) const { return container == it.container && offset() >= it.offset(); }
  bool operator<(const iterator& it) const { return container == it.container && offset() < it.offset(); }
  bool operator>(const iterator& it) const { return container == it.container && offset() > it.offset(); }

  // increment and decrement
  iterator& operator++() { increment(); return *this; }
  iterator& operator--() { decrement(); return *this; }
  iterator operator++(int) { iterator it = *this; increment(); return it; }
  iterator operator--(int) { iterator it = *this; decrement(); return it; }
  iterator operator+=(difference_type d) { advance(+d); return *this; }
  iterator operator-=(difference_type d) { advance(-d); return *this; }

protected:
  using const_iterator::offset;
  using const_iterator::advance;
  using const_iterator::increment;
  using const_iterator::decrement;
  using const_iterator::container;
  using const_iterator::x;
  using const_iterator::y;
  using const_iterator::z;
};
