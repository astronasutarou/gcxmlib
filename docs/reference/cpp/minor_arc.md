# `minor_arc` class

## `minor_arc`

The `minor_arc` class, a descendant of `great_circle`, defines an arc on a sphere. It requires two directions; one is the starting point, and the other is the end point.


### Constructor

#### minor_arc(const direction_cosine& s, const direction_cosine& e)

Create a `minor_arc` from two directions.

s
: the starting point of the arc.

e
: the end point of the arc.


### Functions

#### const double<br>separation_cosine(const great_circle& gc) const

Obtain cos&theta;, where &theta; is the separation angle between another `great_circle` instance. Inherited from `great_circle`.

gc
: another `great_circle` instance.

#### const angle<br>separation(const great_circle& gc) const

Obtain &theta;, where &theta; is the separation angle between another `great_circle` instance. Inherited from `great_circle`.

gc
: another `great_circle` instance.

#### const double<br>separation_cosine(const direction_cosine& p) const

Obtain cos&theta;, where &theta; is the separation angle between the point *p* and the foot of the perpendicular from *p* to this instance. Inherited from `great_circle`.

p
: a `direction_cosine` instance.

#### const angle<br>separation(const direction_cosine& p) const

Obtain &theta;, where &theta; is the separation angle between the point *p* and the foot of the perpendicular from *p* to this instance. Inherited from `great_circle`.

p
: a `direction_cosine` instance.

#### const direction_cosine<br>foot_of(const direction_cosine& p) const

Obtain the foot of the perpendicular from *p* to this instance. Inherited from `great_circle`.

p
: a `direction_cosine` instance.

!!! caution
    When *p* is identical to the pole, the foot cannot be specified. This function, however, does not throw an exception. Instead, it returns the foot from either (1,0,0) or (0,1,0).


#### const double<br>distance_cosine(const direction_cosine& p) const

Calculate cos&theta;, where &theta; is the separation angle between the nearest point on the arc and the point *p*. Note that the difference from `separation_cosine()`, which returns the separation angle to the great circle.

p
: a `direction_cosine` instance.

#### const angle<br>distance(const direction_cosine& p) const

Calculate &theta;, where &theta; is the separation angle between the nearest point on the arc and the point *p*. Note that the difference from `separation_cosine()`, which returns the separation angle to the great circle.

p
: a `direction_cosine` instance.

#### const direction_cosine<br>extrapolate(const double f) const

Calculate the extrapolated direction along the arc. This function returns the starting and end points for *f*=0.0 and *f*=1.0, respectively. Thus, the function interpolates the arc in case that *f* is between 0.0 and 1.0.

f
: an extrapolation factor.

#### void<br>dump(const size_t N=64) const

List the points on the circle in `stdout`. Inherited from `great_circle`.

N
: the number of vertices to be listed.

#### void<br>dump_arc(const size_t N=64) const

List the points on the arc in `stdout`.

N
: the number of vertices to be listed.


### Attributes

pole
: (direction_cosine) the pole of the arc. Inherited from `great_circle`.

s
: (direction_cosine) the starting point of the arc.

e
: (direction_cosine) the end point of the arc.
