cmake_minimum_required(VERSION 3.20)
project(json-pointer-loc-test)

add_executable(json-pointer-loc-test test_main.c munit/munit.c ../json-pointer-loc.c)
