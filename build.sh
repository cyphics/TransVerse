#!/bin/bash

start_compile="$(date -u +%s.%N)"
ctags -e -R .
compiled=false

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
    cd build/
    cmake ../
    if cmake --build .
    then
        compiled=true
    fi
}

#build
build_cmake

end_compile="$(date -u +%s.%N)"
elapsed_compile="$(bc <<<"$end_compile-$start_compile")"
echo ""
echo "Program compiled in $elapsed_compile"

if [ "$compiled" = true ]; then
    ./TransVerse
fi

# ./tests
# ./tests "Times to strings"
# ./tests "RemainingTime()"

exit 0
