FFTW3
======
[![License: GPL v2](https://img.shields.io/badge/License-GPL%20v2-blue.svg)](https://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)



This is a fork of the [FFTW3](https://github.com/FFTW/fftw3) for the Armv8-A 64-bit architecture (AArch64) with 512-bit Scalable Vector Extension (SVE) support.

See the [FFTW](http://www.fftw.org/) home page for more information.

# Prerequisites

* Fujitsu Fortran Compiler and Fujitsu C/C++ Compiler
* GNU automake 1.16.1 or higher
* GNU Autoconf 2.69 or higher
* GNU libtool 2.4.6 or higher
* GNU Texinfo 6.7 or higher

# Installation

## Download the source code
* Clone this repository.

```
$ git clone https://github.com/fujitsu/fftw3.git
```

## Generate configure scripts
* Run autoreconf.

```
$ cd fftw3
$ touch ChangeLog
$ autoreconf --verbose --install --symlink --force
```

## Building FFTW3

### Single precision
* Native compilation using Fujitsu compiler (AArch64 target).

```
$ ./configure                        \
    CC="fcc"                         \
    F77="frt"                        \
    CFLAGS='-Nclang -Ofast'          \
    FFLAGS='-Kfast'                  \
    --enable-sve                     \
    --enable-armv8-cntvct-el0        \
    --enable-float                   \
    --enable-fma                     \
    --enable-fortran                 \
    --enable-openmp                  \
    --enable-shared                  \
    --prefix="$INSTALL_PATH"         \
    --libdir="$INSTALL_PATH/lib64"   \
    ac_cv_prog_f77_v='-###'          \
    OPENMP_CFLAGS='-Kopenmp'
$ make -j30
```

* Cross compilation using Fujitsu compiler (AArch64 target).

```
$ ./configure                        \
    CC="fccpx"                       \
    F77="frtpx"                      \
    CFLAGS='-Nclang -Ofast'          \
    FFLAGS='-Kfast'                  \
    --host=aarch64-unknown-linux-gnu \
    --build=x86_64-cross-linux-gnu   \
    --enable-sve                     \
    --enable-armv8-cntvct-el0        \
    --enable-float                   \
    --enable-fma                     \
    --enable-fortran                 \
    --enable-openmp                  \
    --enable-shared                  \
    --prefix="$INSTALL_PATH"         \
    --libdir="$INSTALL_PATH/lib64"   \
    ac_cv_prog_f77_v='-###'          \
    OPENMP_CFLAGS='-Kopenmp'
$ make -j30
```

### Double precision
* Native compilation using Fujitsu compiler (AArch64 target).

```
$ ./configure                        \
    CC="fcc"                         \
    F77="frt"                        \
    CFLAGS='-Nclang -Ofast'          \
    FFLAGS='-Kfast'                  \
    --enable-sve                     \
    --enable-armv8-cntvct-el0        \
    --enable-fma                     \
    --enable-fortran                 \
    --enable-openmp                  \
    --enable-shared                  \
    --prefix="$INSTALL_PATH"         \
    --libdir="$INSTALL_PATH/lib64"   \
    ac_cv_prog_f77_v='-###'          \
    OPENMP_CFLAGS='-Kopenmp'
$ make -j30
```

* Cross compilation using Fujitsu compiler (AArch64 target).

```
$ ./configure                        \
    CC="fccpx"                       \
    F77="frtpx"                      \
    CFLAGS='-Nclang -Ofast'          \
    FFLAGS='-Kfast'                  \
    --host=aarch64-unknown-linux-gnu \
    --build=x86_64-cross-linux-gnu   \
    --enable-sve                     \
    --enable-armv8-cntvct-el0        \
    --enable-fma                     \
    --enable-fortran                 \
    --enable-openmp                  \
    --enable-shared                  \
    --prefix="$INSTALL_PATH"         \
    --libdir="$INSTALL_PATH/lib64"   \
    ac_cv_prog_f77_v='-###'          \
    OPENMP_CFLAGS='-Kopenmp'
$ make -j30
```


## Run tests

```
$ make check
```

## Install FFTW3

```
$ make install
```

# Usage

## How to compile
* Compile with the -L$(INSTALL_PATH)/lib64 option and one of the options
  listed in the following table after your sources or objects.

| FFTW3 libraries                   　　　　　  | Options                   |
| --------------------------------------------- | ------------------------- |
| Single precision and sequential version       | -lfftw3f -lm              |
| Single precision and thread-parallel version  | -lfftw3f_omp -lfftw3f -lm |
| Double precision and sequential version　　   | -lfftw3 -lm               |
| Double precision and thread-parallel version  | -lfftw3_omp -lfftw3 -lm   |

## Example

* Case with the single precision and sequential library.

```
$ frt a.f -L$(INSTALL_PATH)/lib64 -lfftw3f -lm
```

# License
* See [LICENSE](https://github.com/fujitsu/fftw3/blob/fj_master/COPYING) file.

