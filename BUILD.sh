#!/bin/bash
set -e

here="$(realpath "$(dirname "$0")")"
cd "$here"

bin=bin
include=include
src=src
docs=docs

language='--std=c11 --pedantic -Wall -fwrapv'
optimize='-O2'
compile="gcc $language $optimize -c"
tcHeader="gcc $language $optimize -o /dev/null"


headers=''
headers="$headers sanity"
headers="$headers chimtypes"
headers="$headers buffer/byte"
headers="$headers buffer/boxed"
headers="$headers slice/byte"

modules=''
modules="$modules alignment"
modules="$modules alloc/unaligned"
modules="$modules alloc/aligned"
modules="$modules alloc/tags"
modules="$modules buffer"
modules="$modules slice"

trap "rm -f delme.c" EXIT

for header in $headers; do
  mkdir -p "$(dirname "$include/$header")"
  echo "#include \"$header.h\"" > delme.c
  echo "int main(){return 0;}" >> delme.c
  $tcHeader -I "$src" delme.c
  cp "$src/$header.h" "$include/$header.h"
done

for module in $modules; do
  mkdir -p "$(dirname "$bin/$module")" "$(dirname "$include/$module")"
  $compile -I "$src" "$src/$module.c" -o "$bin/$module.o"
  cp "$src/$module.h" "$include/$module.h"
done


doxygen "$docs/Doxyfile"
