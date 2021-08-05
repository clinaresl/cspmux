# Introduction #

`cspmux` is a constraint satsifaction (CSP) solver based on binary mutex. It is implemented in C++ and distributed as a library.

`cspmux` defines a *Constraint Satisfaction Task* as a triplet \( (X, D, M) \) where:

* $X$ is the set of /variables/

* $D$ is the set of domains of each variable so that $$D_i$$ is the set of plausible values of the /i/-th variable $$x_i$$

* $$M$$ is the set of mutex (mutually exclusive) pairs: $$(d_i^u)$$ is mutex with $$(d_j^v)$$ if and only if $$x_i$$ and $$x_j$$ can not take simultaneously the values $$d^u$$ and $$d^v$$ respectively.


# Dependencies #

None


# Install #

To compile the `cspmux` library you can use the following command:

```bash
   $ cmake
```

`cmake`, will go through a number of tests and, if it does not find any error, it will automatically build the library.

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
   $ make distclean
```   


# Tests #

The unit tests have been defined using Google Test. To compile all test cases and run the tests, type:

```bash
   $ make check
```    

from the root directory where the library has been installed. In case all tests are enabled, a failure might be reported for all those unit tests that require accessing files below the directory `tests`. To make sure the tests are performed correctly it is strongly adviced to execute the following commands:

```bash
   $ cd tests
   $ ./gtest --gtest_also_run_disabled_tests
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

