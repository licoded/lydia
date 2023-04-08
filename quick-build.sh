set -e

cd /home/licoded/lydia
rm -rf build
mkdir build
cd build
cmake -Wno-dev -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target lydia-bin -j16
make install
