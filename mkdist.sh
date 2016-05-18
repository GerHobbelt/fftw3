# This is a script used by the FFTW authors to generate FFTW distributions.
# You should never need to use it.

NJOBS=4

# hackery to build ChangeLog
git log --pretty=medium --date-order > ChangeLog

sh bootstrap.sh

make maintainer-clean
./configure --enable-maintainer-mode --enable-single --enable-sse --enable-threads
make -j $NJOBS
make -j $NJOBS dist

