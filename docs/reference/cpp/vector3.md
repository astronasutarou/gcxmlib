# `vector3` class

## `vector3`

The `vector3` class defines a 3-dimensional vector. Several calculations between `vector3` instances are defined. The constructor of `vector3` requires an (*x*, *y*, *z*) coordinate.


### Constructor

#### vector3(const double x, const double y, const double z)
Create a `vector3` instance from a coordinate.

x
: *x*-coordinate.

y
: *y*-coordinate.

z
: *z*-coordinate.


### Functions

#### const double<br>inner_product(const vector3& v) const

Calculate an inner product with another `vector3` instance `v`.

v
: a `vector3` instance.


#### const vector3<br>outer_product(const vector3& v) const

Calculate an outer product with another `vectore` instance `v`.

v
: a `vector3` instance.

#### const double<br>separation_cosine(const vector3& v) const

Calculate cos&theta;, where &theta; is the separation angle between another `vector3` instance `v`.

v
: a `vector3` instance.


#### const angle<br>separation(const vector3& v) const

Calculate &theta;, where &theta; is the separation angle between another `vector3` instance `v`.

v
: a `vector3` instance.

#### void<br>print() const

Write down the (*x*,*y*,*z*) coordinate in `stdout`.


### Attributes

x
: (double) *x*-coordinate.

y
: (double) *y*-coordinate.

z
: (double) *z*-coordinate.

d
: (double) length of the vector.
