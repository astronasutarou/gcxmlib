# `great_circle` class

## `great_circle`

The `great_circle` class defines a circle or line on a sphere. It has a `direction_cosine` instance, the direction of the pole, which characterizes the `great_circle`.


### Constructors

#### great_circle(const direction_cosine& p)

Create a `great_circle` instance with the pole of *p*.

p
: a `direction_cosine` instance, the pole direction.


#### great_circle(const double l, const double m, const double n)

Create a `great_circle` instance with the pole (*l*,*m*,*n*).

l
: *l*-element of the pole direction.

m
: *m*-element of the pole direction.

n
: *n*-element of the pole direction.

#### great_circle(const longitude& lon, const latitude& lat)

Create a `great_circle` instance with the pole (*lon*, *lat*).

lon
: the longitude of the pole direction.

lat
: the latitude of the pole direction.


### Functions

#### const double<br>separation_cosine(const great_circle& gc) const

Obtain cos&theta;, where &theta; is the separation angle between another `great_circle` instance.

gc
: another `great_circle` instance.

#### const angle<br>separation(const great_circle& gc) const

Obtain &theta;, where &theta; is the separation angle between another `great_circle` instance.

gc
: another `great_circle` instance.

#### const double<br>separation_cosine(const direction_cosine& p) const

Obtain cos&theta;, where &theta; is the separation angle between the point *p* and the foot of the perpendicular from *p* to this instance.

p
: a `direction_cosine` instance.

#### const angle<br>separation(const direction_cosine& p) const

Obtain &theta;, where &theta; is the separation angle between the point *p* and the foot of the perpendicular from *p* to this instance..

p
: a `direction_cosine` instance.

#### const direction_cosine<br>foot_of(const direction_cosine& p) const

Obtain the foot of the perpendicular from *p* to this instance.

p
: a `direction_cosine` instance.

!!! caution
    When *p* is identical to the pole, the foot cannot be specified. This function, however, does not throw an exception. Instead, it returns the foot from either (1,0,0) or (0,1,0).

#### void<br>print(const size_t N=64) const

List the points on the circle in `stdout`.

N
: the number of vertices to be listed.


### Attributes

pole
: (direction_cosine) the pole of the great circle.
