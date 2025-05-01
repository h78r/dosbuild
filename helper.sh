git submodule update --init --recursive
sudo dnf install gcc-c++ bison flex texinfo patch zlib-devel tar bzip2 gzip xz unzip python3-devel m4 dos2unix nasm which
compiler/tools/build-gcc/build-djgpp.sh --prefix=/opt/djgpp djgpp-cvs binutils gcc-14.2.0
