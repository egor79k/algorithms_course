#!/bin/bash

# Args:
# 1 - login
# 2 - alloc_type
# 3 - svn link

echo "::::EXPERIMENT FOR $1::::"
# mkdir experiments
cd experiments
# svn co https://github.com/egor79k/algorithms_course/trunk/kiselev_e_l
svn co $3
cp m3i/m3i.h m3i/m3i.cpp kiselev_e_l/prj.lab/m3i
rm -r -f m3i
cd kiselev_e_l
# mkdir build
cd build
cmake ..
make m3i_alloc_time
cd ../..
./kiselev_e_l/build/prj.lab/m3i/m3i_alloc_time $1 $2
