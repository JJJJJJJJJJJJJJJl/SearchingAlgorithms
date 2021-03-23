# SearchingAlgorithms

Compile:<br/>
```
g++ -Wall main.cpp -o main
```
Execute:<br/>
```
./main
```
Input:<br/>

First line asks for a number N of points.<br/>
Second line asks for a flag number, 1 so it reads N points from stdin or 2 so randomly it generates N points.<br/>
In case of flag being 1, a point must be written as "x y".<br/>
It then prints out each algorithm/heuristic solution.<br/>
<br/>Input test example:<br/>
```
5
1
1 9
9 -9
-4 -7
7 7
8 -7
```
<br/>Possible output solution:<br/>
```
Permutation Solution: (1,9) (9,-9) (-4,-7) (8,-7) (7,7)

Nearest Neighbour Solution: (-4,-7) (8,-7) (9,-9) (7,7) (1,9)

2-Exchange Neighbourhood:
[(8,-7);(-4,-7)] [(1,9);(9,-9)] [(9,-9);(-4,-7)] [(1,9);(7,7)] [(7,7);(8,-7)]
[(8,-7);(1,9)] [(1,9);(-4,-7)] [(9,-9);(-4,-7)] [(9,-9);(7,7)] [(7,7);(8,-7)]
[(8,-7);(1,9)] [(1,9);(7,7)] [(9,-9);(-4,-7)] [(-4,-7);(7,7)] [(9,-9);(8,-7)]

Hill Climbing (based on different heuristics):
Best-Improvement First: [(8,-7);(7,7)] [(1,9);(7,7)] [(9,-9);(-4,-7)] [(-4,-7);(1,9)] [(9,-9);(8,-7)]

First-Improvement: [(8,-7);(-4,-7)] [(1,9);(8,-7)] [(9,-9);(-4,-7)] [(1,9);(7,7)] [(7,7);(9,-9)]

Less Edge Intersections: [(8,-7);(1,9)] [(1,9);(-4,-7)] [(9,-9);(-4,-7)] [(9,-9);(7,7)] [(7,7);(8,-7)]

Random Neighbour: [(8,-7);(7,7)] [(1,9);(7,7)] [(9,-9);(-4,-7)] [(-4,-7);(1,9)] [(9,-9);(8,-7)]
```
