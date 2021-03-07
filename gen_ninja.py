#!/usr/bin/env python3
import os
import glob

objs = ''
target = 'build build/test/test: link_test'

sources = [
    'src/writer.c',
    'src/midi.c',
    'src/parse.c',
    'src/token.c',
    'src/vec.c',
]
tests = glob.glob('test/test_*.cpp')

test_build_dir = 'build/test'

for file in tests+sources:
    path = os.path.dirname(file)
    base = os.path.basename(file)
    name, ext = os.path.splitext(base)
    objname = os.path.join(test_build_dir, name + '.o')
    objs += f'build {objname}: compile_test {file}\n'
    target += f' {objname}'
    
ninja = """cxxflags = -g -O0 -std=c++11
cxx = g++

rule compile_test 
  command = $cxx $cppflags -c $in -I include -o $out
rule link_test
  command = $cxx $in -o $out -I include -lgtest -lgtest_main -lpthread

{objs}

{target}

build test: phony build/test/test

default test

""".format(objs=objs, target=target)

f = open('build.ninja','w')
f.write(ninja)
f.close()

