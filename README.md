# SearchingAlgorithms

**Goal**<br/>
Given a sequence of 2D points forming a polygon, it aims to find the minimal cost [hamilton cycle](https://en.wikipedia.org/wiki/Hamiltonian_path).<br/> 

**Compile**<br/>
```
g++ -Wall main.cpp -o main
```
**Execute**<br/>
```
./main
```
**Input**<br/>

First line asks for a number N of points.<br/>
Second line asks for a flag number, 1 so it reads N points from stdin or 2 so it randomly generates N points.<br/>
In case of flag being 1, a point must be written as "x y".<br/>
In case of flag being 2, a number M is then asked meaning coordinate values will be constrained to [-M..M].<br/>
<br/>**Input Test Example**<br/>
```
5
1
1 9
9 -9
-4 -7
7 7
8 -7
```
<br/>**Possible output solution**<br/>
```
Permutation Solution: (1,9) (9,-9) (-4,-7) (8,-7) (7,7)

Nearest Neighbour Solution: (-4,-7) (8,-7) (9,-9) (7,7) (1,9)

2-Exchange Neighbourhood:
(1,9) (-4,-7) (9,-9) (7,7) (8,-7)
(1,9) (7,7) (-4,-7) (9,-9) (8,-7)
(1,9) (9,-9) (-4,-7) (8,-7) (7,7)

Hill Climbing (based on different heuristics):
Best-Improvement First: (1,9) (7,7) (8,-7) (9,-9) (-4,-7)

First-Improvement: (1,9) (-4,-7) (9,-9) (7,7) (8,-7)

Less Edge Intersections: (1,9) (-4,-7) (9,-9) (7,7) (8,-7)

Random Neighbour: (1,9) (7,7) (8,-7) (9,-9) (-4,-7)
```
