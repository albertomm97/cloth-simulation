rmdir /s/q build-vs2019
mkdir build-vs2019
cd build-vs2019

cmake -G "Visual Studio 16 2019" ..

cd ..
