# `angle` class


## `base_angle`

A template class `base_angle` provides functions to handle angles in different scales. The `base_angle` class requires the `angle_range` class as a template parameter, which defines the domain of the class. There are four `angle_range` options defined.

``` c++
enum class angle_range
{
  zero_to_twopi,       /** range [0, 2pi)      (w/wrap)   */
  minus_pi_to_pi,      /** range [-pi, pi)     (w/wrap)   */
  zero_to_pi,          /** range [0, pi]       (w/o wrap) */
  minus_pi_2_to_pi_2   /** range [-pi/2, pi/2] (w/o wrap) */
};
```

`zero_to_twopi` is a default option. It produces the `base_angle` with the domain of [0, 2&pi;). If one requires the angle symmetric about zero, `minus_pi_to_pi` is available, which produces the `base_angle` with the domain of [-&pi;, &pi;). `zero_to_pi` generates the `base_angle` with the domain of [0, &pi;]. It is useful to define the angle between two directions. `minus_pi_2_to_pi_2` gives the domain of [-&pi;/2, &pi;/2] to define the `latitude`.


### Constructor

#### base_angle&lt;angle_range range&gt;(double r)
Create a `base_angle` instance with *r* radian.

range
: the domain of `base_angle`.

r
: an angle in degree.


### Attributes

radian
: (double) angle in radian.

degree
: (double) angle in degree.

arcmin
: (double) angle in arcminute.

arcsec
: (double) angle in arcsecond.


### Operators

#### operator double() const
A cast of an `angle` instance always provides an angle in degree.

#### const base_angle&lt;range&gt;<br>operator-() const
Returns an instance with an negative angle.

#### const base_angle&lt;range&gt;<br>operator+(const base_angle&lt;range&gt;& ang, const double val)
Define the `add` operator between `base_angle` and `double`.

#### const base_angle&lt;range&gt;<br>operator+(const double val, const base_angle&lt;range&gt;& ang)
Define the `add` operator between `double` and `base_angle`.

#### const base_angle&lt;range&gt;<br>operator-(const base_angle&lt;range&gt;& ang, const double val)
Define the `subtract` operator between `base_angle` and `double`.

#### const base_angle&lt;range&gt;<br>operator-(const double val, const base_angle&lt;range&gt;& ang)
Define the `subtract` operator between `double` and `base_angle`.

#### const base_angle&lt;range&gt;<br>operator*(const base_angle&lt;range&gt;& ang, const double val)
Define the `product` operator between `base_angle` and `double`.

#### const base_angle&lt;range&gt;<br>operator*(const double val, const base_angle&lt;range&gt;& ang)
Define the `product` operator between `double` and `base_angle`.

#### const base_angle&lt;range&gt;<br>operator/(const base_angle&lt;range&gt;& ang, const double val)
Define the `divide` operator between `base_angle` and `double`.

#### const base_angle&lt;range&gt;<br>operator/(const double val, const base_angle&lt;range&gt;& ang)
Define the `divide` operator between `double` and `base_angle`.


#### const base_angle&lt;range&gt;<br>operator+(const base_angle&lt;_range&gt;& ang) const
Define the `add` operator with another `base_angle` instance.

#### const base_angle&lt;range&gt;<br>operator-(const base_angle&lt;_range&gt;& ang) const
Define the `subtract` operator with another `base_angle` instance.

#### const bool<br>operator==(const base_angle&lt;_range&gt;& ang) const
Define `equality` operator to anotehr `base_angle` instance.

#### const bool<br>operator!=(const base_angle&lt;_range&gt;& ang) const
Define `inequality` operator to anotehr `base_angle` instance.

#### const bool<br>operator&lt;(const base_angle&lt;_range&gt;& ang) const
Define `less` operator to anotehr `base_angle` instance.

#### const bool<br>operator&lt;=(const base_angle&lt;_range&gt;& ang) const
Define `less-or-equal` operator to anotehr `base_angle` instance.

#### const bool<br>operator&gt;(const base_angle&lt;_range&gt;& ang) const
Define `greater` operator to anotehr `base_angle` instance.

#### const bool<br>operator&gt;=(const base_angle&lt;_range&gt;& ang) const
Define `greater-or-equal` operator to anotehr `base_angle` instance.


---

Some specializations of `base_angle` are defined as follows.

- `angle`
- `longitude`
- `latitude`

## `angle`

``` c++
/** general purpose angle class. */
using angle = base_angle<angle_range::zero_to_twopi>;
```
The `angle` is a general-purpose specialization of `base_angle`.  The domain of the `angle` is 0 to 2&pi;. The argument outside of the domain is automatically wrapped into [0, 2&pi;).


## `longitude`

``` c++
/** `longitude` is an alias to `angle`. */
using longitude = base_angle<angle_range::zero_to_twopi>;
```
The `longitude` is just an alias of `angle`. The domain of the `longitude` is 0 to 2&pi;.


## `latitude`

``` c++
/** `latitude` is defined within [-pi/2,pi/2]. */
using latitude = base_angle<angle_range::minus_pi_2_to_pi_2>;
```

The latitude is another specialization of base_angle. The domain of the latitude is -&pi;/2 to &pi;/2. The argument outside of the domain throws an exception.
