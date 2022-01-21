# `matrix3` class

## `matrix3`

### Constructors

#### matrix3()

Create an identity matrix.

#### matrix3(const std::array&lt;double,9&gt; arr)

Create a `matrix3` instance with an array.

arr
: An array contains 9 elements.

### Functions

#### const matrix3<br>T() const

Returns the transposed `matrix3` instance.

#### void<br>print() const

Display all the elements in `stdout`.


### Operators

#### const double<br>operator[] (const size_t i) const

Access to the *i*-th element of the matrix.

$$
A =
\begin{pmatrix}
A[0] & A[1] & A[2] \\
A[3] & A[4] & A[5] \\
A[6] & A[7] & A[8]
\end{pmatrix}
$$

#### const double<br>operator() (const size_t i, const size_t j) const

Access to the (*i,j*)-element of the matrix.

$$
A =
\begin{pmatrix}
A(0,0) & A(0,1) & A(0,2) \\
A(1,0) & A(1,1) & A(1,2) \\
A(2,0) & A(2,1) & A(2,2)
\end{pmatrix}
$$

#### const positional<br>operator* (const positional& v) const
Defines the `product` operator between `matrxi3` and a positional class. The positional class is either `vector3` or `direction_cosine`.

#### const footprint<br>operator* (const footprint& v) const
Defines the `product` operator between `matrxi3` and `footprint`.

#### const matrix3<br>operator- () const
Returns a `matrix3` with all the elements negated.

#### const matrix3<br>operator+ (const matrix3& m) const
Defines the `add` operator between another `matrix3` instance.

#### const matrix3<br>operator- (const matrix3& m) const
Defines the `subtract` operator between another `matrix3` instance.

#### const matrix3<br>operator* (const matrix3& m) const
Defines the `product` operator between another `matrix3` instance.

#### const matrix3<br>operator+ (const matrix3& m, const double x)
Defines the `add` operator between `matrix3` and `double`.

#### const matrix3<br>operator+ (const double x, const matrix3& m)
Defines the `add` operator between `double` and `matrix3`.

#### const matrix3<br>operator- (const matrix3& m, const double x)
Defines the `subtract` operator between `matrix3` and `double`.

#### const matrix3<br>operator- (const double x, const matrix3& m)
Defines the `subtract` operator between `double` and `matrix3`.

#### const matrix3<br>operator* (const matrix3& m, const double x)
Defines the `product` operator between `matrix3` and `double`.

#### const matrix3<br>operator* (const double x, const matrix3& m)
Defines the `product` operator between `double` and `matrix3`.

#### const matrix3<br>operator/ (const matrix3& m, const double x)
Defines the `divide` operator between `matrix3` and `double`.
