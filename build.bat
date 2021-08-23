The Flower Collectors@echo off

set "build_dir=build\"
md "%build_dir%"

rem cmake --build "%build_dir%" --target transverse -- -j 6

cd "%build_dir%"
cmake ..
cmake --build .
