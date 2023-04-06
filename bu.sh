set -e # exit after first ERROR

alias ll="ls -la"
cd /build/lydia
rm -rf build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --target lydia-bin -j4
make install
ll /usr/local/bin/lydia
