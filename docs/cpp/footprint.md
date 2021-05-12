# `footprint` class

## `footprint`

The `footprint` class is a descendant of the `direction_cosine` class., which defines the point on a unit spare with a timestamp and uncertainty.


### Constructors

#### footprint(const double l, const double m, const double n)

Create a `footprint` with a direction cosine of (*l*, *m*, *n*). The current system time is registered as a timestamp. A default uncertainty value, 1.0 arcsecond, is adopted.

l
: *l*-coordinate

m
: *m*-coordinate

n
: *n*-coordinate

#### footprint(const double x, const double y, const double z, const timestamp_t& t, const angle& s = arcsec(1.0))

Create a `footprint` with a direction cosine of (*l*, *m*, *n*, *t*, *s*), where *t* is the timestamp and *s* is the uncertainty. The uncertainty can be skipped. If skipped, a default uncertainty value, 1.0 arcsecond, is adopted.

l
: *l*-coordinate

m
: *m*-coordinate

n
: *n*-coordinate

t
: the timestamp.

s
: the uncertainty.

#### footprint(const longitude& lon, const latitude& lat, const timestamp_t& t, const angle& s = arcsec(1.0))

Create a `footprint` with a direction cosine, timestamp, and uncertainty. The direction cosine is defined by (*lon*, *lat*), where *lon* is the longitude and *lat* is the latitude. The uncertainty can be skipped. If skipped, a default uncertainty value, 1. arcsecond is adopted.

lon
: the longitude angle.

lat
: the latitude angle.

t
: the timestamp.

s
: the uncertainty.


### Functions

#### const double<br>inner_product(const vector3& v) const

Calculate an inner product with another `vector3` instance `v`. Inherited from the `direction_cosine` class.

v
: a `vector3` instance.


#### const vector3<br>outer_product(const vector3& v) const

Calculate an outer product with another `vectore` instance `v`. Inherited from the `direction_cosine` class.

v
: a `vector3` instance.

#### const double<br>separation_cosine(const vector3& v) const

Calculate cos&theta;, where &theta; is the separation angle between another `vector3` instance `v`. Inherited from the `direction_cosine` class.

v
: a `vector3` instance.


#### const angle<br>separation(const vector3& v) const

Calculate &theta;, where &theta; is the separation angle between another `vector3` instance `v`. Inherited from the `direction_cosine` class.

v
: a `vector3` instance.

#### const direction_cosine<br>get_pole(const vector3& v) const

Calculate the pole direction perpendicular to both this instance and a `vector3` instance. Inherited from the `direction_cosine` class.

v
: a `vector3` instance.

#### const direction_cosine<br>pivot(const direction_cosine& q, const angle& f1, const angle& f2) const

Obtain the direction which is separated by *f1* from this instance and separated by *f2* from another `direction_cosine` *q*. Inherited from the `direction_cosine` class.

q
: an anchor point.

f1
: the separation angle from this instance.

f2
: the separation angle from the anchor point *q*.

#### const footprint<br>extend_to(const footprint& q, const angle& d) const

Obtain an extended `footprint` point toward *q*. The separation angle from this instance is specified by *d*. Override the function inherited from the `direction_cosine` class.

q
: a `footprint` instance, which defines the direction to extend.

d
: a separation angle between this instance and the extended point.

#### const footprint<br>extend_to(const footprint& q, const double f) const

Obtain an extended `footprint` point toward *q*. The separation from this instance is specified by *f*, a separation normalized by the distance between this instance and *q*. Override the function inherited from the `direction_cosine` class.

q
: a `footprint` instance, which defines the direction to extend.

f
: a separation factor.

#### const bool<br>neighbor_to(const vector3& v, const angle& range) const

Return `true` if the `vector3` instance is located within `range`.

v
: a `vector3` instance. Internally converted into `direction_cosine`.

range
: a search radius angle.

#### const bool<br>neighbor_to(const vector3& v) const

Return `true` if the `vector3` instance is located within the uncertainty of this instance.

v
: a `vector3` instance. Internally converted into `direction_cosine`.

#### const bool<br>neighbor_to(const footprint& p) const

Return `true` if the `footprint` instance is located within the uncertainty. The uncertainties of both instances are taken into account.

p
: a `footprint` instance.

#### const bool<br>match(const footprint& p, const angle& range, const sec_t& trange) const

Return `true` if the footprint *p* is considered as identical to this instance. The search radius is specified by `range`, while the search time window is given by `trange`.

p
: a `footprint` instance.

range
: a search radius.

#### void<br>dump() const

Write down the (*l*,*m*,*n*) coordinate in `stdout` as well as the uncertainty in arcsecond and the timestamp in the ISO format.


### Attributes

l
: (double) *l*-element of the direction cosine. Inherited from `direction_cosine`.

m
: (double) *m*-element of the direction cosine. Inherited from `direction_cosine`.

n
: (double) *n*-element of the direction cosine. Inherited from `direction_cosine`.

t
: (timestamp_t) the timestamp of the direction cosine.

lon
: (longitude) the longitude angle of the direction cosine. Inherited from `direction_cosine`.

lat
: (latitude) the latitude angle of the direction cosine. Inherited from `direction_cosine`.

x
: (double) the reference to the *l*-element of the direction cosine. Inherited from `direction_cosine`.

y
: (double) the reference to the *m*-element of the direction cosine. Inherited from `direction_cosine`.

z
: (double) the reference to the *n*-element of the direction cosine. Inherited from `direction_cosine`.

d
: (double) always unity. Inherited from `direction_cosine`.
