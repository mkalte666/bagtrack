#!/bin/bash

cd assets/ || exit 1
rm -f ./*.cpp
cd .. || exit 1

for f in assets/*; do
  cppfile=$f.cpp
  echo "converting $f"
  xxd -i "$f" > "$cppfile"
done