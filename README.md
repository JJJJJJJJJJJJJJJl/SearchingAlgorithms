# SearchingAlgorithms

**Goal**<br/>
Given a sequence of 2D points the goal is to find a simple polygon.<br/> 
Featuring some algorithms, as listed:<br/>
		- Permutation<br/>
		- Nearest Neighbour<br/>
		- Hill Climbing through different heuristics (2-opt neighbourhood)<br/>
		- Simulated Annealing (2-opt neighbourhood)<br/>
		- Ant Colony Optimization<br/>

**Note**: Goal (simple polygon) might not be achieved. 

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
 - Best-Improvement First: (1,9) (7,7) (8,-7) (9,-9) (-4,-7)

 - First-Improvement: (1,9) (-4,-7) (9,-9) (7,7) (8,-7)

 - Less Edge Intersections: (1,9) (-4,-7) (9,-9) (7,7) (8,-7)

 - Random Neighbour: (1,9) (7,7) (8,-7) (9,-9) (-4,-7)

Simulated Annealing: (1,9) (-4,-7) (9,-9) (7,7) (8,-7)

Ant Colony Optimization: (8,-7) (9,-9) (-4,-7) (1,9) (7,7)
```
