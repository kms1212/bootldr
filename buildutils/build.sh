export PREFIX="$PWD/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

REQUIRED_PKG="wget build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo"

BUVER="2.35"
GCCVER="10.2.0"

SH_HOME="$PWD"

cd $SH_HOME


# Configuring packages for building

sudo apt-get --yes install $REQUIRED_PKG


# Downloading src files for building cross-compiler

echo "Receiving archive files : "
if ! [ -f "binutils-$BUVER.tar.gz" ] ; then
	wget "ftp://ftp.gnu.org/gnu/binutils/binutils-$BUVER.tar.gz"
fi
if ! [ -f "gcc-$GCCVER.tar.gz" ] ; then
	wget "ftp://ftp.gnu.org/gnu/gcc/gcc-$GCCVER/gcc-$GCCVER.tar.gz"
fi

echo "Unpacking archive files :"
if ! [ -d "binutils-$BUVER" ] ; then
	tar -xvf "binutils-$BUVER.tar.gz"
fi
if ! [ -d "gcc-$GCCVER" ] ; then
	tar -xvf "gcc-$GCCVER.tar.gz"
fi

mv "binutils-$BUVER" "binutils"
mv "gcc-$GCCVER" "gcc"


# Building binutils

echo "Building binutils : "

rm -rf binutils-build
mkdir binutils-build
cd binutils-build
../binutils/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror

make
make install
cd $SH_HOME

# Building GCC

echo "Building GCC : "

rm -rf gcc-build
mkdir gcc-build
cd gcc-build
../gcc/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
cd $SH_HOME
