# `angle` class


## `base_angle`

The `gcxmlib` provides a template class `base_angle`.




Some specializations of `base_angle` are defined as follows. For general purposes, the `angle` is available. The `longitude` is just an alias of `angle`. For angles constrained between -90&deg; and 90&deg;, the `latitude` is provided.


## `angle`

``` c++
/** general purpose angle class. */
using angle = base_angle<angle_range::zero_to_twopi>;
```


## `longitude`

``` c++
/** `longitude` is an alias to `angle`. */
using longitude = base_angle<angle_range::zero_to_twopi>;
```


## `latitude`

``` c++
/** `latitude` is defined within [-pi/2,pi/2]. */
using latitude = base_angle<angle_range::minus_pi_2_to_pi_2>;
```
