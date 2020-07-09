#!/bin/bash

start_compile="$(date -u +%s.%N)"
ctags -e -R .

# -Wno-narrowing -Wno-enum-compare are fore  raygui.h

build()
{
    if gcc -x c -g -Wno-write-strings \
          -o build/TransVerse \
          -lm -lraylib \
          src/main.cpp -Ilib/stb/
    then
        compiled=true
    else
	    compiled=false
    fi
}

build

end_compile="$(date -u +%s.%N)"
elapsed_compile="$(bc <<<"$end_compile-$start_compile")"
echo ""
echo "Program compiled in $elapsed_compile"

if [ "$compiled" = true ]; then
    cd build
    ./TransVerse
fi

# ./tests
# ./tests "Times to strings"
# ./tests "RemainingTime()"

exit 0
