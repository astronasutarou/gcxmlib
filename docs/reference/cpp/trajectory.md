# `trajectory` class

## `trajectory`

### Constructors

#### trajectory(const trail& t)

Construct a trajectory instance from a single trail instance.

t
: the `trail` instance.

#### trajectory(const std::initializer_list<trail> list)

Construct a trajectory instance from a list of `trail` instances.

list
: the list of `trail` instances.

#### trajectory(const std::vector<trail> list)

Construct a trajectory instance from a list of `trail` instances.

list
: the vector array containing `trail` instances.


### Functions

#### const trail&<br>arc() const

Obtain a reference to the current `trail` arc.

#### const footprint&<br>s() const

Obtain a reference to the starting point of the current `trail` arc.

#### const footprint&<br>e() const

Obtain a reference to the end point of the current `trail` arc.

#### const std::vector<trail>&<br>list() const

Obtain a reference to the vector array of the `trail` instances.

#### void<br>append(const trail& t)

Append a single `trail` instance.

t
: the `trail` instance.

#### void<br>append(const std::initializer_list<trail> list)

Append `trail` instances in the list.

list
: the list of `trail` instances.

#### void<br>append(const std::vector<trail> list)

Append `trail` instances in the vector array.

list
: the vector array of `trail` instances.

#### const double<br>separation_cosine(const great_circle& gc) const

Obtain cos&theta;, where &theta; is the separation angle between a `great_circle` instance and the trail arc of this instance. This function takes into account the uncertainty of the trail arc.

gc
: another `great_circle` instance.

#### const angle<br>separation(const great_circle& gc) const

Obtain &theta;, where &theta; is the separation angle between a `great_circle` instance and the trail arc of this instance. This function takes into account the uncertainty of the trail arc.

gc
: another `great_circle` instance.

#### const double<br>separation_cosine(const direction_cosine& p) const

Obtain cos&theta;, where &theta; is the separation angle between the point *p* and the foot of the perpendicular from *p* to the trail arc of this instance. This function takes into account the uncertainty of the trail arc.

p
: a `direction_cosine` instance.

#### const angle<br>separation(const direction_cosine& p) const

Obtain &theta;, where &theta; is the separation angle between the point *p* and the foot of the perpendicular from *p* to the trail arc of this instance. This function takes into account the uncertainty of the trail arc.

p
: a `direction_cosine` instance.

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

#### const footprint<br>propagate(const sec_t& dT, const double R = 100.0) const

Extrapolate the trail arc by the time span of *dT*. The uncertainty is calculated based on the uncertainties of the arc. This function partly takes into account the acceleration and deceleration of the trails.

dT
: a propagation time span since the end point.

R
: an optional regularization parameter to take into account acceleration.

#### const footprint<br>propagate(const timestamp_t& T, const double R = 100.0) const

Extrapolate the trail arc to the time point *T*. The uncertainty is calculated based on the uncertainties of the arc. This function partly takes into account the acceleration and deceleration of the trails.

T
: a time point of the extrapolated point.


R
: an optional regularization parameter to take into account acceleration.

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

#### const bool<br>intersect_with(const trajectory& trj) const

Return `true` if a `trail` *arc* crosses the uncertainty region of the trail arc.

trj
: a `trajectory` instance.

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

#### const bool<br>colinear_with(const trajectory& trj, const angle& tol = degree(5.0)) const

Return `true` when the separation angle between the poles is smaller than *tol*. The uncertainties are taken into account.

arc
: a `trajectory` instance.

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

#### const bool<br>match(const trajectory& trj, const angle& dtol = degree(5.0), const angle& rtol = arcmin(5.0), const double margin = 1.0) const

Return `true` when another `trajectory` instance is consistent with the current one. First, the distance between the poles of both instances should be smaller than *rtol*. Then, the current trail arc is extrapolated to the time of the given trail. The extrapolated starting and end points are compared with those of the given arc. The separation angles between the end points should be smaller than *dtol*, respectively.

trj
: another `trajectory` instance.

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

List the points on the circle in `stdout`.

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
