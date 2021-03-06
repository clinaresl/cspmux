August, 5, 2021

# Introduction #

`cspmux` is a constraint satsifaction (CSP) solver based on binary mutexes. It is implemented in C++ and distributed as a library.

`cspmux` defines a *Constraint Satisfaction Task* as a triplet *(X, D, M)* where:

* *X* is the set of *variables*

* *D* is the set of *domains* of each variable so that *Di* is the set of plausible *values* of the *i*-th variable *xi*

* *M* is the set of mutexes (mutually exclusive pairs): two variables *xi* and *xj*  have a mutex *(di, dj)* if and only if they can not take simultaneously the values *di* and *dj* respectively.


# Dependencies #

Tests have been created using the [Google Test Framework](https://github.com/google/googletest) which is necessary for both compiling and runing the tests, see below.


# Install #

To download the software clone this repository:

``` bash
    $ git clone https://github.com/clinaresl/cspmux.git
```

To compile the `cspmux` library, the tests and all the examples, cd to `cspmux`
and compile all parts with:

```bash
   $ cd cspmux/
   $ cmake --build .
```

To compile only the `cspmux` library cd to `cspmux/src` and compile the code:

```bash
   $ cd cspmux/src/
   $ cmake --build .
```

To install the library just type:

```bash
   $ make install
```    

By default, the library is installed in `/usr/local/lib` and the header files are installed in `/usr/local/include/libdot-MAJOR.MINOR` (where *MAJOR* and *MINOR* are substituted by the version numbers of this library) To change the include and lib dirs use `--prefix` with `./configure`:

```bash
   $ ./configure --prefix=/home/myuser/local; make
```

It will automatically create a `Makefile` which is specifically instructed to copy all the binaries and headers in `/home/myuser/local`

To uninstall the binaries and headers of the `cspmux` library just execute:

```bash
   $ make uninstall
```    

Be aware that it might be necessary to prepend both `make install` and `make uninstall` with `sudo` in case you are installing in su-protected directories such as `/usr/local`.

Finally, to entirely remove the intermediate files from the installation directory type:

```bash
   $ cmake --build . --target=clean
```   


# Tests #

The unit tests have been defined using Google Test. To compile separately the
unit test cases type the following from `cspmux/`:

```bash
   $ cd tests/
   $ cmake --build .
```    

To run the tests:

```bash
   $ tests/gtest
```    

# Documentation #

All the documentation has been generated with Sphinx. To regenerate the documentation type:

```bash
   $ make html
```    

from the `doc/` directory to generate the documentation in HTML format. The main HTML page can be found at `_build/html/index.html`; and type:

```bash
   $ make latexpdf
```    
from the `doc/` directory to generate the same documentation as a PDF. Note that this requires `pdflatex` to be installed in your system.

Finally, to entirely remove the intermediate files from the `doc/` directory type:

```bash
   $ make clean
```   


# License #

`cspmux` is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

`cspmux` is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with `cspmux`.  If not, see <http://www.gnu.org/licenses/>.


# Author #

Carlos Linares Lopez <carlos.linares@uc3m.es>  
Computer Science Department  
Universidad Carlos III de Madrid

