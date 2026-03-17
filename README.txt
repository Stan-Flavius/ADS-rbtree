How to run:

requirements:
  a C++11 (or later) compiler: g++ 7+ or clang++ 6+ both work
  all files must be in the same directory

build:
  g++ -std=c++11 -o rbtree main.cpp
  g++ -std=c++11 -o leaderboard leaderboard.cpp
  g++ -std=c++11 -o median median.cpp

run:
  ./rbtree      (or rbtree.exe)
  ./leaderboard (or leaderboard.exe)
  ./median      (or median.exe)
