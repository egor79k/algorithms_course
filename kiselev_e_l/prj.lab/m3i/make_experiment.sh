#!/bin/bash
mkdir experiments
cd experiments
svn co https://github.com/egor79k/algorithms_course/trunk/kiselev_e_l
svn co $3
cp $1/prj.lab/m3i/m3i.h,$1/prj.lab/m3i/m3i.cpp kiselev_e_l/prj.lab/m3i
cd kiselev_e_l
mkdir build
cd build
cmake ..
make
./prj.lab/m3i/m3i_alloc_time $1 $2