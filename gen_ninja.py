#!/usr/bin/env python3
import os
import glob

objs = ''
test_bin = 'build build/test/test: link_test'
midori_bin = 'build build/midori/midori: link'


files = glob.glob('src/*.c')
print(files)
skip_list = [
    'main.c',
    'lang.c',
]
sources = list(filter(lambda x: all(skip not in x for skip in skip_list), files))
print(sources)
midori_build_dir = 'build/midori'

for file in sources:
    path = os.path.dirname(file)
    base = os.path.basename(file)
    name, ext = os.path.splitext(base)
    objname = os.path.join(midori_build_dir, name + '.o')
    objs += f'build {objname}: compile {file}\n'
    midori_bin += f' {objname}'

tests = glob.glob('test/test_*.cpp')

test_build_dir = 'build/test'

for file in tests + sources:
    path = os.path.dirname(file)
    base = os.path.basename(file)
    name, ext = os.path.splitext(base)
    objname = os.path.join(test_build_dir, name + '.o')
    objs += f'build {objname}: compile_test {file}\n'
    test_bin += f' {objname}'
    
ninja = f"""
cc = gcc
cflags = -g -O0 -std=c11
cxx = g++
cxxflags = -g -O0 -std=c++11

rule compile
  command = $cc $cflags -o $out $in -I include -c
rule link
  command = $cc $cflags -o $out $in -I include src/lang.c

rule compile_test 
  command = $cxx $cppflags -c $in -I include -o $out
rule link_test
  command = $cxx $in -o $out -I include -lgtest -lgtest_main -lpthread

{objs}

{midori_bin}
{test_bin}

build midori: phony build/midori/midori
build test: phony build/test/test

default test
"""

f = open('build.ninja','w')
f.write(ninja)
f.close()

