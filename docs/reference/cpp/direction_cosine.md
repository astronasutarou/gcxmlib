# `direction_cosine` class

## `direction_cosine`
The `direction_cosine` class is a descendant of the `vector3` class, with the vector length normalized to unity. The `dcos` is an alias to the `direction_cosine`.

``` c++
using dcos = direction_cosine;
```

### Constructors

#### direction_cosine(const double l, const double m, const double n)

Create a `direction_cosine` instance with a (l,m,n) coordinate.

l
: the *l*-element of the direction cosine.

m
: the *m*-element of the direction cosine.

n
: the *n*-element of the direction cosine.


#### direction_cosine(const vector3& p)

Create a `direction_cosine` instance from another instance.

p
: another `direction_cosine` instance.

#### direction_cosine(const longitude& lon, const latitude& lat)

Create a `direction_cosine` instance from a `longitude` and `latitude` pair.

lon
: a `longitude` instance.

lat
: a `latitude` instance.


### Functions

#### const double<br>inner_product(const vector3& v) const

Calculate an inner product with another `vector3` instance `v`. Inherited from the `vector3` class.

v
: a `vector3` instance.


#### const vector3<br>outer_product(const vector3& v) const

Calculate an outer product with another `vectore` instance `v`. Inherited from the `vector3` class.

v
: a `vector3` instance.

#### const double<br>separation_cosine(const vector3& v) const

Calculate cos&theta;, where &theta; is the separation angle between another `vector3` instance `v`. Inherited from the `vector3` class.

v
: a `vector3` instance.


#### const angle<br>separation(const vector3& v) const

Calculate &theta;, where &theta; is the separation angle between another `vector3` instance `v`. Inherited from the `vector3` class.

v
: a `vector3` instance.

#### const direction_cosine<br>get_pole(const vector3& v) const

Calculate the pole direction perpendicular to both this instance and a `vector3` instance.

v
: a `vector3` instance.

#### const direction_cosine<br>pivot(const direction_cosine& q, const angle& f1, const angle& f2) const

Obtain the direction which is separated by *f1* from this instance and separated by *f2* from another `direction_cosine` *q*.

q
: an anchor point.

f1
: the separation angle from this instance.

f2
: the separation angle from the anchor point *q*.

#### const direction_cosine<br>extend_to(const direction_cosine& q, const angle& d) const

Obtain an extended `direction_cosine` point toward *q*. The separation angle from this instance is specified by *d*.

q
: a `direction_cosine` instance, which defines the direction to extend.

d
: a separation angle between this instance and the extended point.

#### const direction_cosine<br>extend_to(const direction_cosine& q, const double f) const

Obtain an extended `direction_cosine` point toward *q*. The separation from this instance is specified by *f*, a separation normalized by the distance between this instance and *q*.

q
: a `direction_cosine` instance, which defines the direction to extend.

f
: a separation factor.

#### void<br>print() const

Write down the (*l*,*m*,*n*) coordinate in `stdout`.


### Attributes

l
: (double) *l*-element of the direction cosine.

m
: (double) *m*-element of the direction cosine.

n
: (double) *n*-element of the direction cosine.

lon
: (longitude) the longitude angle of the direction cosine.

lat
: (latitude) the latitude angle of the direction cosine.

x
: (double) the reference to the *l*-element of the direction cosine. Inherited from `vector3`.

y
: (double) the reference to the *m*-element of the direction cosine. Inherited from `vector3`.

z
: (double) the reference to the *n*-element of the direction cosine. Inherited from `vector3`.

d
: (double) always unity. Inherited from `vector3`.
