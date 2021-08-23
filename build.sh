#!/bin/bash

start_compile="$(date -u +%s.%N)"
ctags -e -R .
compilation_successful=false
test_suite=false
run_program=true
build_dir="build"

# -Wno-narrowing -Wno-enum-compare are fore  raygui.h

build()
{
    if gcc -x c -g -Wno-write-strings \
          -o build/TransVerse \
          -lm -lraylib \
          src/main.c -Ilib/stb/
    then
        compiled=true
    else
	    compiled=false
    fi
}

build_cmake()
{

    if [ ! -d "build" ]; then
        mkdir build
    fi
	cmake .
	cmake --configure .
    if cmake --build "$build_dir" --target transverse -- -j 6

    # cd build/
    # cmake -Wno-dev ../
    # if cmake --build .
    then
        compilation_successful=true
        if [ "$test_suite" ]
        then
            cmake --build "$build_dir" --target test_suite -- -j 6
        fi
    fi
}

#build
build_cmake

end_compile="$(date -u +%s.%N)"
elapsed_compile="$(bc <<<"$end_compile-$start_compile")"
echo ""
echo "Program compiled in $elapsed_compile"

if [ "$compilation_successful" = true ]; then
    echo
    if [ "$test_suite" = true ]; then
        ./"$build_dir"/tests/test_suite
    fi
    if [ "$run_program" = true ]; then
        cd "$build_dir"/src/
        ./transverse

    fi
fi

# ./tests
# ./tests "Times to strings"
# ./tests "RemainingTime()"

exit 0
