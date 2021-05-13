# `trajectory` class

## `trajectory`

### Constructors

#### trajectory(const trail& t)
#### trajectory(const std::initializer_list<trail> list)
#### trajectory(const std::vector<trail> list)


### Functions

#### const trail&<br>arc() const
#### const footprint&<br>s() const
#### const footprint&<br>e() const
#### const std::vector<trail>&<br>list() const

#### void<br>append(const trail& t)
#### void<br>append(const std::initializer_list<trail> list)
#### void<br>append(const std::vector<trail> list)

#### const double<br>separation_cosine(const great_circle& gc) const
#### const angle<br>separation(const great_circle& gc) const
#### const double<br>separation_cosine(const direction_cosine& p) const
#### const angle<br>separation(const direction_cosine& p) const
#### const double<br>distance_cosine(const direction_cosine& p) const
#### const angle<br>distance(const direction_cosine& p) const
#### const direction_cosine<br>extrapolate(const double f) const
#### const footprint<br>propagate(const sec_t& dT, const double R = 100.0) const
#### const footprint<br>propagate(const timestamp_t& T, const double R = 100.0) const
#### const bool<br>intersect_with(const direction_cosine& p) const
#### const bool<br>intersect_with(const minor_arc& arc) const
#### const bool<br>intersect_with(const trail& arc) const
#### const bool<br>intersect_with(const trajectory& trj) const

#### const bool<br>colinear_with(const great_circle& gc, const angle& tol = degree(5.0)) const
#### const bool<br>colinear_with(const minor_arc& arc, const angle& tol = degree(5.0)) const
#### const bool<br>colinear_with(const trail& arc, const angle& tol = degree(5.0)) const
#### const bool<br>colinear_with(const trajectory& trj, const angle& tol = degree(5.0)) const

#### const bool<br>match(const trail& arc, const angle& dtol = degree(5.0), const angle& rtol = arcmin(5.0), const double margin = 1.0) const
#### const bool<br>match(const trajectory& trj, const angle& dtol = degree(5.0), const angle& rtol = arcmin(5.0), const double margin = 1.0) const

#### const angle<br>error_at(const direction_cosine& q, const bool skip=true) const

#### void<br>dump(const size_t N=64) const
#### void<br>dump_arc(const size_t N=64) const
#### void<br>dump_error(const size_t N=64) const
