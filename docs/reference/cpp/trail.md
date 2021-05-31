# `trail` class

## `trail`

### Constructor

#### trail(const footprint& s, const footprint& e)

s
: the starting point of the trail arc.

e
: the end point of the trail arc.


### Functions

#### const double<br>separation_cosine(const great_circle& gc) const

Obtain cos&theta;, where &theta; is the separation angle between another `great_circle` instance. This function takes into account the uncertainty of the trail arc. Override the function inherited from `great_circle`.

gc
: another `great_circle` instance.

#### const angle<br>separation(const great_circle& gc) const

Obtain &theta;, where &theta; is the separation angle between another `great_circle` instance. This function takes into account the uncertainty of the trail arc. Override the function inherited from `great_circle`.

gc
: another `great_circle` instance.

#### const double<br>separation_cosine(const direction_cosine& p) const

Obtain cos&theta;, where &theta; is the separation angle between the point *p* and the foot of the perpendicular from *p* to this instance. This function takes into account the uncertainty of the trail arc. Override the function inherited from `great_circle`.

p
: a `direction_cosine` instance.

#### const angle<br>separation(const direction_cosine& p) const

Obtain &theta;, where &theta; is the separation angle between the point *p* and the foot of the perpendicular from *p* to this instance. This function takes into account the uncertainty of the trail arc. Override the function inherited from `great_circle`.

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

#### const footprint<br>propagate(const sec_t& dT) const

Extrapolate the trail arc by the time span of *dT*. The uncertainty is calculated based on the uncertainties of the arc.

dT
: a propagation time span since the end point.

#### const footprint<br>propagate(const timestamp_t& T) const

Extrapolate the trail arc to the time point *T*. The uncertainty is calculated based on the uncertainties of the arc.

T
: a time point of the extrapolated point.

#### const bool<br>intersect_with(const direction_cosine& p) const

Return `true` if the point *p* is located within the uncertainty region of the trail arc.

p
: a `direction_cosine` instance.

#### const bool<br>intersect_with(const minor_arc& arc) const

Return `true` if a segment *arc* crosses the uncertainty region of the trail arc.

arc
: a `minor_arc` instance.

#### const bool<br>intersect_with(const trail& arc) const

Return `true` if a `trail` *arc* crosses the uncertainty region of the trail arc.

arc
: a `trail` instance.

#### const bool<br>colinear_with(const great_circle& gc, const angle& tol = degree(5.0)) const

Return `true` when the separation angle between the poles is smaller than *tol*.

gc
: a `great_circle` instance.

tol
: a tolerance of the separation angle.

#### const bool<br>colinear_with(const minor_arc& arc, const angle& tol = degree(5.0)) const

Return `true` when the separation angle between the poles is smaller than *tol*.

arc
: a `minor_arc` instance.

tol
: a tolerance of the separation angle.

#### const bool<br>colinear_with(const trail& arc, const angle& tol = degree(5.0)) const

Return `true` when the separation angle between the poles is smaller than *tol*. The uncertainties are taken into account.

arc
: a `trail` instance.

tol
: a tolerance of the separation angle.

#### const bool<br>match(const trail& arc, const angle& dtol = degree(5.0), const angle& rtol = arcmin(5.0), const double margin = 1.0) const

Return `true` when another `trail` instance is consistent with the current one. First, the distance between the poles of both instances should be smaller than *rtol*. Then, the current trail arc is extrapolated to the time of the given trail. The extrapolated starting and end points are compared with those of the given arc. The separation angles between the end points should be smaller than *dtol*, respectively.

arc
: another `trail` instance.

dtol
: a tolerance angle in the distance between the arcs.

rtol
: a tolerance angle in the separation between the poles.


#### const angle<br>error_at(const direction_cosine& q, const bool skip=true) const

Calculate the uncertainty at the foot of the perpendicular from *q* to the great circle.

q
: a `direction_cosine` instance, a reference point.

skip
: set true if `q` is certainly on the great circle of this instance.


#### void<br>print(const size_t N=64) const

List the points on the circle in `stdout`. Inherited from `great_circle`.

N
: the number of vertices to be listed.

#### void<br>print_arc(const size_t N=64) const

List the points on the arc in `stdout`.

N
: the number of vertices to be listed.

#### void<br>print_error(const size_t N=64) const

List the uncertainty region in `stdout`. This function outputs *N* pairs of `footprint` instances which define the boundary of the uncertainty region. The output format is optimized for the `splot` grid of `gnuplot`.

N
: the number of partitions to be listed.

### Attributes

pole
: (direction_cosine) the pole of the arc. Inherited from `great_circle`.

s
: (footprint) the starting point of the trail arc.

e
: (footprint) the end point of the trail arc.

dt
: (sec_t) the duration of the arc.
