# gcxmlib

This library provides some classes and functions to cross-match tracklets on a sphere.


## Dependences
The library is developed along with the C++14 standard. Test programs are compiled and checked with `g++-5.4.0`.


## Installation


### Python
The python package `gcxmlib` is available on PyPI (*not available yet*). Otherwise, you can install a latest version via the git repository on bitbucket.org (*not available yet*).

``` sh
pip install gcxmlib
pip install git+https:://bitbucket.org/ryou_ohsawa/gcxmlib/src/master/
```

In case that the installation via `pip` fails, you can download the whole code from bitbucket.org and install the module manually.

``` sh
cd /your/favorite/directory
git clone https://ryou_ohsawa@bitbucket.org/ryou_ohsawa/gcxmlib.git
cd gcxmlib
python ./setup.py install
```

After the installation, you can import the library as usual.

``` python
import gcxmlib
```


### C++
The C++ library is provided as a single header file. Copy to your developing directory and add the include line as follows. Do not forget to add `-I` option in compiling a program.

``` c++
include "gcxmlib.h"
```
