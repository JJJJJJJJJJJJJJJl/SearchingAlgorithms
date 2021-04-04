#include <bits/stdc++.h>
using namespace std;

//prints points as they appear in vector
void show_vector_points(vector<pair<int,int>> points){
    int n = (int)points.size();
    for(int i=0; i<n; i++){
        cout << "(" << points[i].first << "," << points[i].second << ")";
        if(i != n-1){
            cout << " ";
        }
    }
    cout << endl;
    return;
}

//here we do euclidean distance as (x1-x2)^2 + (y1-y2)^2 instead of the square root of squares sum
//to avoid floating point approximation
int euclidean_distance(pair<int,int> p1, pair<int,int> p2){
    return ((p1.first - p2.first) * (p1.first - p2.first)) + ((p1.second - p2.second) * (p1.second - p2.second));
}

//checks if two points are different
int points_different(pair<int,int> p1, pair<int,int> p2){
    return p1.first != p2.first || p1.second != p2.second;
}

//checks if p3 is within p1p2 bounds
int in_box(pair<int,int> p1, pair<int,int> p2, pair<int,int> p3){
    return min(p1.first, p2.first) <= p3.first && max(p1.first, p2.first) >= p3.first
        && min(p1.second, p2.second) <= p3.second && max(p1.second, p2.second) >= p3.second; 
}

//checks intersection between two edges (p1p2 & p3p4)
int edges_intersect(pair<int,int> p1, pair<int,int> p2, pair<int,int> p3, pair<int,int> p4){
    // |_ can happen since this kind of intersection is valid in polygons 
    //so if theres two identical points then either they're the same edge or look like this |_ meaning they will never intersect
    if(points_different(p1,p3) && points_different(p1,p4) && points_different(p2,p3) && points_different(p2,p4)){

        //checking traversing turn signal (clockwise or counter-clockwise)
        int d12_3 = ((p3.first - p1.first) * (p2.second - p1.second)) - ((p3.second - p1.second) * (p2.first - p1.first));
        int d12_4 = ((p4.first - p1.first) * (p2.second - p1.second)) - ((p4.second - p1.second) * (p2.first - p1.first));
        int d34_1 = ((p1.first - p3.first) * (p4.second - p3.second)) - ((p1.second - p3.second) * (p4.first - p3.first));
        int d34_2 = ((p2.first - p3.first) * (p4.second - p3.second)) - ((p2.second - p3.second) * (p4.first - p3.first));

        //both straddle tests passed, p1p2 -|- p3p4 
        if((d12_3 * d12_4) < 0 && (d34_1 * d34_2) < 0){
            return 1;
        }

        //p3 belongs to p1p2 segment |-
        else if(d12_3 == 0 && in_box(p1, p2, p3)){
            return 1;
        }
        //p4 belongs to p1p2 segment |-
        else if(d12_4 == 0 && in_box(p1, p2, p4)){
            return 1;
        }
        //p1 belongs to p3p4 segment |-
        else if(d34_1 == 0 && in_box(p3, p4, p1)){
            return 1;
        }
        //p2 belongs to p3p4 segment |-
        else if(d34_2 == 0 && in_box(p3, p4, p2)){
            return 1;
        }
    }
    //p1p2 | | p3p4
    return 0;
}

//efficient alternative to pow() props:https://stackoverflow.com/questions/26860574/pow-implementation-in-cmath-and-efficient-replacement
double powerd (double x, int y){
    double temp;
    if (y == 0){
        return 1;
    }
    temp = powerd(x, y / 2);
    if((y % 2) == 0){
        return temp * temp;
    }
    else{
        if(y > 0){
            return x * temp * temp;
        }
        else{
            return (temp * temp) / x;
        }
    }
}

//not the perimeter exactly since we calculating euclidean distance without square root the squares sum to avoid floating point approximations
int perimeter(vector<pair<pair<int,int>,pair<int,int>>> edges){
    int ans = 0;
    for(int i=0; i<(int)edges.size(); i++){
        ans += euclidean_distance(edges[i].first, edges[i].second);
    }
    return ans;
}

//given n points generaste n edges by the points order
void generate_edges(vector<pair<int,int>> points, vector<pair<pair<int,int>,pair<int,int>>> &vector_edges){
    int n = (int) points.size();
    //generating edges from initial disposition
    for(int i=1; i<n; i++){
        pair<pair<int,int>,pair<int,int>> p = make_pair(points[i-1], points[i]);
        vector_edges.push_back(p);
    }
    vector_edges.push_back(make_pair(points[n-1], points[0]));
    return;
}

//scalar product of two segments
int scalar_product(pair<pair<int,int>,pair<int,int>> a, pair<pair<int,int>,pair<int,int>> b){
    pair<int,int> a_vector = make_pair(a.second.first - a.first.first, a.second.second - a.first.second);
    pair<int,int> b_vector = make_pair(b.second.first - b.first.first, b.second.second - b.first.second);
    return ((a_vector.first * b_vector.first) + (a_vector.second * b_vector.second));
}

//checks direction and collinearity between two segments
int collinear_opposite_direction_edges(pair<pair<int,int>,pair<int,int>> a, pair<pair<int,int>,pair<int,int>> b){
    double a_slope;
    double b_slope;
    double c_slope;

    //vertical line, there's no slope
    if((a.second.first - a.first.first) == 0){
        a_slope = 42069;
    }
    else{
        a_slope = fabs((a.second.second - a.first.second) / (a.second.first - a.first.first));
    }

    //vertical line, there's no slope
    if((b.second.first - b.first.first) == 0){
        b_slope = 42069;
    }
    else{
        b_slope = fabs((b.second.second - b.first.second) / (b.second.first - b.first.first));
    }

     //vertical line, there's no slope
    if((b.first.first - a.second.first) == 0){
        c_slope = 42069;
    }
    else{
        c_slope = fabs((b.first.second - a.second.second) / (b.first.first - a.second.first));
    }

    //edges collinear and lay in opposite directions
    if(a_slope == b_slope && b_slope == c_slope && a_slope == c_slope && scalar_product(a, b) < 0){
        return 1;
    }

    //they're not
    return 0;
}

//number of intersections given a cycle
int intersections(vector<pair<pair<int,int>,pair<int,int>>> vector_edges){
    int n = (int) vector_edges.size();
    set<pair<pair<pair<int,int>,pair<int,int>>,pair<pair<int,int>,pair<int,int>>>> vi;//visited intersections
    int ans = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i != j && edges_intersect(vector_edges[i].first, vector_edges[i].second, vector_edges[j].first, vector_edges[j].second) 
                    && !collinear_opposite_direction_edges(vector_edges[i], vector_edges[j])
                    && vi.find(make_pair(vector_edges[i], vector_edges[j])) == vi.end() 
                    && vi.find(make_pair(vector_edges[j], vector_edges[i])) == vi.end()){
                        ans++;
                        vi.insert(make_pair(vector_edges[i], vector_edges[j]));
                        vi.insert(make_pair(vector_edges[j], vector_edges[i]));
            }
        }
    }
    return ans;
}

//classic dfs
void dfs(int flag, int x, vector<int> graph[], int visited[], map<int,pair<int,int>> point_index_reversed){
    visited[x] = 1;
    if(flag == 2){
        cout << "(" << point_index_reversed[x].first << "," << point_index_reversed[x].second << ") "; 
    }
    for(int i=0; i<(int)graph[x].size(); i++){
        if(!visited[graph[x][i]]){
            dfs(flag, graph[x][i], graph, visited, point_index_reversed);
        }
    }
    return;
}

//flag == 1, simply checks if graph's connected
//flag == 2, printing purposes (show_vector_edges, so instead of printing edges like "[[(x1,y1).(x2,y2)]]" it prints the actual points)
int is_it_connected(int flag, vector<pair<pair<int,int>,pair<int,int>>> edges){
    //assigning each point an index
    map<pair<int,int>,int> point_index;
    map<int,pair<int,int>> point_index_reversed;
    int index = 0;
    for(int i=0; i<(int)edges.size(); i++){
        if(point_index.find(edges[i].first) == point_index.end()){
            point_index[edges[i].first] = index;
            if(flag == 2){
                point_index_reversed[index] = edges[i].first; 
            }
            index++;
        }
        if(point_index.find(edges[i].second) == point_index.end()){
            point_index[edges[i].second] = index;
            if(flag == 2){
                point_index_reversed[index] = edges[i].second;
            }
            index++;
        }
    }

    //creating the adjacency list               
    vector<int> graph[index+1];
    for(int i=0; i<(int)edges.size(); i++){
        graph[point_index[edges[i].first]].push_back(point_index[edges[i].second]);
        graph[point_index[edges[i].second]].push_back(point_index[edges[i].first]);
    }

    //dfs through point 0 (could be any)
    //then checking visited array, if all were visited(1) then it is connected
    int visited[index+1];
    memset(visited, 0, sizeof(visited));
    if(flag == 1){
        dfs(1, 0, graph, visited, point_index_reversed);
        for(int i=0; i<index; i++){
            if(visited[i] == 0){
                return 0;
            }
        }
    }
    else{
        dfs(2, 0, graph, visited, point_index_reversed);
    }
    return 1;
}

//display edges as points connection through converting edges into a graph and printing it with dfs
void show_vector_edges(vector<pair<pair<int,int>,pair<int,int>>> edges){
    is_it_connected(2, edges);
    cout << endl;
    return;
}

//flag == 0 -> Generates all legit neighbours.
//flag == 1 -> Returns state with lowest perimeter ("best-improvement first").
//flag == 2 -> Returns the first state it finds with lower perimeter than the current state ("first-improvement").
//flag == 3 -> Returns state with less edge intersections.
void two_exchange(int flag, vector<pair<pair<int,int>,pair<int,int>>> vector_edges, vector<vector<pair<pair<int,int>,pair<int,int>>>> &neighbourhood){
    int lowest_perimeter = perimeter(vector_edges);;//flag == 1/2 purposes
    int lowest_intersections = intersections(vector_edges);//flag == 3 purposes
    int n = (int) vector_edges.size();
    set<pair<pair<pair<int,int>,pair<int,int>>,pair<pair<int,int>,pair<int,int>>>> vi;//visited intersections
    set<pair<pair<int,int>,pair<int,int>>> set_edges;//edges of original disposition (finding purposes)

    //filling set_edges up
    for(int i=0; i<n; i++){
        set_edges.insert(vector_edges[i]);
    }
    //if(flag == 0) show_vector_edges(vector_edges);
    //generating neighbourhood by 2-exchange
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(vector_edges[i] != vector_edges[j]){

                //if edges intersect
                //if edges are not collinear while laying in opposite directions
                //if intersection havent been seen already 
                if(edges_intersect(vector_edges[i].first, vector_edges[i].second, vector_edges[j].first, vector_edges[j].second) 
                    && !collinear_opposite_direction_edges(vector_edges[i], vector_edges[j])
                    && vi.find(make_pair(vector_edges[i], vector_edges[j])) == vi.end() 
                    && vi.find(make_pair(vector_edges[j], vector_edges[i])) == vi.end()){

                    //{A,B} {C,D} -> {A,C} {B,D} 
                    pair<int,int> temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].first;
                    vector_edges[j].first = temp;

                    //reversing within path purposes
                    int left = -1;
                    int right = -1;

                    //if new edges dont already exist
                    if(set_edges.find(vector_edges[i]) == set_edges.end() 
                    && set_edges.find(make_pair(vector_edges[i].second, vector_edges[i].first)) == set_edges.end()
                    && set_edges.find(vector_edges[j]) == set_edges.end()
                    && set_edges.find(make_pair(vector_edges[j].second, vector_edges[j].first)) == set_edges.end()
                    && is_it_connected(1, vector_edges)){

                        //reversing path within intersecting edges
                        if(abs(i - j) > 1){
                            if(i > j){
                                left = j+1;
                                right = i-1;
                            }
                            else{
                                left = i+1;
                                right = j-1;
                            }
                            //so left n right are saved allowing the opposite operation
                            int left_to_reverse = left;
                            int right_to_reverse = right;

                            while(left <= right){
                                pair<int,int> temp = vector_edges[left].first;
                                vector_edges[left].first = vector_edges[left].second;
                                vector_edges[left].second = temp;

                                temp = vector_edges[right].first;
                                vector_edges[right].first = vector_edges[right].second;
                                vector_edges[right].second = temp;

                                if(left != right){
                                    pair<pair<int,int>,pair<int,int>> temp2 = vector_edges[left];
                                    vector_edges[left] = vector_edges[right];
                                    vector_edges[right] = temp2;
                                }
                                left++;
                                right--;
                            }
                            //left n right recovered 
                            left = left_to_reverse;
                            right = right_to_reverse;
                        }

                        neighbourhood.push_back(vector_edges);
                    }
                    //reseting to original disposition
                    temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].first;
                    vector_edges[j].first = temp;

                    //reseting reversed path
                    if(left != -1 && right != -1){
                        while(left <= right){
                            pair<int,int> temp = vector_edges[left].first;
                            vector_edges[left].first = vector_edges[left].second;
                            vector_edges[left].second = temp;

                            temp = vector_edges[right].first;
                            vector_edges[right].first = vector_edges[right].second;
                            vector_edges[right].second = temp;

                            if(left != right){
                                pair<pair<int,int>,pair<int,int>> temp2 = vector_edges[left];
                                vector_edges[left] = vector_edges[right];
                                vector_edges[right] = temp2;
                            }
                            left++;
                            right--;
                        }
                    }

                    //{A,B} {C,D} -> {A,D} {C,B}
                    temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].second;
                    vector_edges[j].second = temp;

                    //not so sure how this case should be reversed, couldnt find a single test case where
                    //this one didnt disconnect the path, so here's probably a bug. 

                    //if new edges dont already exist
                    if(set_edges.find(vector_edges[i]) == set_edges.end() 
                    && set_edges.find(make_pair(vector_edges[i].second, vector_edges[i].first)) == set_edges.end()
                    && set_edges.find(vector_edges[j]) == set_edges.end()
                    && set_edges.find(make_pair(vector_edges[j].second, vector_edges[j].first)) == set_edges.end()
                    && is_it_connected(1, vector_edges)){

                        neighbourhood.push_back(vector_edges);
                    }
                    //reseting to original disposition
                    temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].second;
                    vector_edges[j].second = temp;

                    if(flag == 1){
                        int cur_perimeter = perimeter(neighbourhood[(int)neighbourhood.size()-1]);
                        if(cur_perimeter < lowest_perimeter){
                            if((int)neighbourhood.size() > 1){
                                neighbourhood.erase(neighbourhood.begin());
                            }
                            lowest_perimeter = cur_perimeter;
                        }
                        else{
                            neighbourhood.erase(neighbourhood.begin() + (int)neighbourhood.size()-1);
                        }
                    }

                    if(flag == 2){
                        if((int) neighbourhood.size() > 0 && perimeter(neighbourhood[0]) < lowest_perimeter){
                            return;
                        }
                        else{
                            neighbourhood.clear();
                        }
                    }

                    if(flag == 3){
                        int cur_intersections = intersections(neighbourhood[(int)neighbourhood.size()-1]);
                        if(cur_intersections < lowest_intersections){
                            if((int)neighbourhood.size() > 1){
                                neighbourhood.erase(neighbourhood.begin());
                            }
                            lowest_intersections = cur_intersections;
                        }
                        else{
                            neighbourhood.erase(neighbourhood.begin() + (int)neighbourhood.size()-1);
                        }
                    }

                    vi.insert(make_pair(vector_edges[i], vector_edges[j]));
                    vi.insert(make_pair(vector_edges[j], vector_edges[i]));
                }
            }
        }
    }
    return;
}

void hill_climbing(int flag, vector<pair<pair<int,int>,pair<int,int>>> initial){
    vector<vector<pair<pair<int,int>,pair<int,int>>>> temp; 
    vector<pair<pair<int,int>,pair<int,int>>> cur;

    while(1){
        //initial iteration
        if(cur.size() == 0){
            two_exchange(flag, initial, temp);
        }
        //any other iteration 
        else{
            two_exchange(flag, cur, temp);
        }

        //if there are no neighbours to choose from
        if(temp.size() == 0){
            break;
        }

        //how next candidate is chosen
        if(flag == 1 || flag == 2 || flag == 3){
            cur = temp[0];
        }
        else{
            cur = temp[(rand() % (((int)temp.size()-1) + 1))];
        }

        temp.clear();
    }

    if(flag == 1){
        cout << " - Best-Improvement First: ";
        show_vector_edges(cur);
    }
    else if(flag == 2){
        cout << " - First-Improvement: ";
        show_vector_edges(cur);
    }
    else if(flag == 3){
        cout << " - Less Edge Intersections: ";
        show_vector_edges(cur);
    }
    else{
        cout << " - Random Neighbour: ";
        show_vector_edges(cur);
    }
    cout << endl;
    return;
}

//simulated annealing acceptance probability function
int P(int chosen_neighbour_energy, int current_state_energy, double T){
    //energy difference
    int diff = chosen_neighbour_energy - current_state_energy;

    //random number within [0..1}
    double random = (rand() / double(RAND_MAX));

    //accepts candidate 
    if(diff < 0 || random < exp(-diff / T)){
        return 1;
    }
    //denies candidate
    return 0;
}

//energy meaning number of intersections
void simulated_annealing(vector<pair<pair<int,int>,pair<int,int>>> initial){
    vector<vector<pair<pair<int,int>,pair<int,int>>>> current_candidates;
    vector<vector<pair<pair<int,int>,pair<int,int>>>> new_candidates;
    vector<pair<pair<int,int>,pair<int,int>>> cur;
    vector<pair<pair<int,int>,pair<int,int>>> last;
    vector<pair<pair<int,int>,pair<int,int>>> best_sa;
    int best_sa_perimeter;

    last = initial;

    //best measure found
    best_sa_perimeter = perimeter(initial);

    /* NOTES:

    - noticing that algorithm's always stopping at 121 step due to temperature,
      lowering max_steps to something less than 121 makes it stops at that specific value,
      not sure how things could be balanced 

    - also noticing P function causing some floating point exceptions, not really sure why and how (!!!) 

    QUESTIONS:

    - should we search the next candidate from all the neighbours that have been found but not P-tested,
        or should the neighbourhood be restricted to the state neighbours only???????????
    - should a denied candidate be removed from possible candidates??????????
    */
    
    //arbitrary constants
    //temperature
    double T = 1000;
    //temperature cooling rate
    double cooling_rate = 0.95;
    //maximum number of iterations
    int max_steps = 150;
    //current iteration step
    int cur_step = 0;

    while(cur_step != max_steps && T > 2){
        new_candidates.clear();

        //initial neighbours
        //first algorithm iteration
        if((int)cur.size() == 0 && cur_step == 0){
            two_exchange(0, initial, new_candidates);
        }

        //middle time neighbours
        //any other algorithm iteration goes here
        else if((int)cur.size() > 0 && cur_step != 0){
            two_exchange(0, cur, new_candidates);
        }

        //adding new candidates to possible (current) candidates
        for(int i=0; i<(int)new_candidates.size(); i++){
            current_candidates.push_back(new_candidates[i]);
        }

        if((int) current_candidates.size() > 0){
            int rand_index;
            int last_energy = intersections(last);

            while(1){
                rand_index = (rand() % (((int)current_candidates.size()-1) + 1));
                cur = current_candidates[rand_index];
                int cur_energy = intersections(cur);
                
                //candidate accepted
                if(P(cur_energy, last_energy, T)){
                    current_candidates.erase(current_candidates.begin() + rand_index);
                    break;
                }
                
                //candidate denied
                else{
                    //removing denied candidate
                    //if you deny a candidate, should it be removed from possible candidates???
                    if((int)current_candidates.size() > 0){
                        current_candidates.erase(current_candidates.begin() + rand_index);
                    }
                    //no more candidates to remove/choose from
                    else{
                        break;
                    }
                    cur.clear();
                }
            }

            //since we selecting from any candidate it has been found in any state neighbourhood
            int cur_perimeter = perimeter(cur);
            if(cur_perimeter < best_sa_perimeter){
                best_sa_perimeter = cur_perimeter;
                best_sa = cur;
            }

            cur_step++;
            T *= cooling_rate;
            last = cur;
        }
        else{
            break;
        }
    }
    cout << "Simulated Annealing: ";
    show_vector_edges(best_sa);
    cout << endl;
    return;
}

void permutation(vector<pair<int,int>> vector_points){
    vector<pair<int,int>> permutation = vector_points;
    next_permutation(permutation.begin(), permutation.end());
    cout << "Permutation: ";
    show_vector_points(permutation);
    cout << endl;
    return;
}

void nearest_neighbour(int n, set<pair<int,int>> set_points){
    //initial node will always be the one with smallest x-value point, if draw, its ordered by y-value
    int visited[n];
    memset(visited, 0, sizeof(visited));
    vector<pair<int,int>> nnf_ans;
    set<pair<int,int>> help = set_points;

    nnf_ans.push_back(* help.begin());
    help.erase(help.begin());
    while((int) nnf_ans.size() != n){
        pair<int,int> cur = nnf_ans[nnf_ans.size()-1];
        pair<int,int> closest_to_cur;
        int min_dist = INT_MAX;
        for(pair<int,int> p : help){
            int cur_dist = euclidean_distance(cur, p); 
            if(cur_dist < min_dist){
                min_dist = cur_dist;
                closest_to_cur = p;
            }
        }
        help.erase(help.find(closest_to_cur));
        nnf_ans.push_back(closest_to_cur);
    }
    cout << "Nearest Neighbour: ";
    show_vector_points(nnf_ans);
    cout << endl;
    return;
}

//ant colony optimization
void ant_colony(vector<pair<int,int>> points, set<pair<int,int>> set_points){
    int n = (int) points.size();

    vector<pair<pair<int,int>,pair<int,int>>> edges;
    set<pair<pair<int,int>,pair<int,int>>> visited_edges;

    //edge pheromone
    map<pair<pair<int,int>,pair<int,int>>,double> edge_pheromone;

    //best measures found
    int best_perimeter =INT_MAX;
    vector<pair<int,int>> best_ant_path;

    //arbitrary constants
    //maximum number of iterations
    int max_iters = 100;
    //how much found paths influences probability
    int Q = 1;
    //how much edge pheromone influences probability
    int alpha = 5;
    //how much edge distance influences probability
    int beta = 1;
    //pheromone evaporation rate
    double p = 0.85;

    /*
    NOTES:

        - should the algorithm stop once it finds the first simple polygon???????
            stopping definitely decreases time spent but optimal solution might not be returned..hmm dont know what to do 
    */

    //generating all possible edges
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i != j){
                pair<pair<int,int>,pair<int,int>> new_edge = make_pair(points[i], points[j]);
                pair<pair<int,int>,pair<int,int>> new_edge_reversed = make_pair(points[j], points[i]);
                if(visited_edges.find(new_edge) == visited_edges.end() && visited_edges.find(new_edge_reversed) == visited_edges.end()){
                   edges.push_back(new_edge);
                   visited_edges.insert(new_edge);
                   visited_edges.insert(new_edge_reversed); 

                   //setting inital pheronome as 1
                   edge_pheromone[new_edge] = 1;
                   edge_pheromone[new_edge_reversed] = 1;
                }
            }
        }
    }
    visited_edges.clear();

    int ants = 100;
    vector<pair<int,int>> ant_path[ants];

    while(max_iters > 0){
        //randomly choosing initial point
        pair<int,int> initial = points[(rand() % ((n-1) + 1))];

        //generating each ant path
        for(int i=0; i<ants; i++){
            ant_path[i].push_back(initial);
            set<pair<int,int>> help_set_points = set_points;
            help_set_points.erase(help_set_points.find(initial));
            while((int)ant_path[i].size() != n){
                pair<int,int> cur_point = ant_path[i][(int)ant_path[i].size()-1];
                if(help_set_points.size() == 1){
                    ant_path[i].push_back(* help_set_points.begin());
                    help_set_points.erase(help_set_points.begin());
                }
                else{
                    //calculating each edge odd
                    map<pair<int,int>, double> odd;
                    for(pair<int,int> p_i : help_set_points){
                        double numerator = powerd(edge_pheromone[make_pair(cur_point, p_i)], alpha) * powerd((double) 1/euclidean_distance(cur_point, p_i), beta);
                        double denominator = 0; 
                        for(pair<int,int> p_j : help_set_points){
                            denominator += powerd(edge_pheromone[make_pair(cur_point, p_j)], alpha) * powerd((double) 1/euclidean_distance(cur_point, p_j), beta);
                        }   
                        odd[p_i] = numerator / denominator;
                    }

                    //choosing next node based on calculated probability
                    vector<double> odds;
                    map<int,pair<int,int>> index_point;
                    int in = 0;
                    for(auto i : odd){
                        odds.push_back(i.second);
                        index_point[in++] = i.first;
                    }
                    vector<double> cumulative_sum;
                    for(int j=0; j<(int)odds.size(); j++){
                        double cur_sum = odds[j];
                        for(int k=j+1; k<(int)odds.size(); k++){
                            if(k >= (int)odds.size()){
                                break;
                            }
                            cur_sum += odds[k];
                        }
                        cumulative_sum.push_back(cur_sum);
                    }
                    cumulative_sum.push_back((double) 0);
                    double rand_num = (rand() / double(RAND_MAX));
                    for(int j=0; j<(int)cumulative_sum.size()-1; j++){
                        //next node chosen
                        //the difference between each if is on the '>='
                        //rand_num can be 0.00 so we check as >= in the last 2 numbers
                        //example: [1.00, 0.24, 0.05, 0.00]
                        //                             ** rand num <= 0.05 && rand_num >= 0
                        if(j < (int)cumulative_sum.size()-2){
                            if(rand_num <= cumulative_sum[j] && rand_num > cumulative_sum[j+1]){
                                ant_path[i].push_back(index_point[j]);
                                help_set_points.erase(help_set_points.find(index_point[j]));
                                break;
                            }
                        }
                        else{
                            if(rand_num <= cumulative_sum[j] && rand_num >= cumulative_sum[j+1]){
                                ant_path[i].push_back(index_point[j]);
                                help_set_points.erase(help_set_points.find(index_point[j]));
                                break;
                            } 
                        }   
                    }
                }
            }
        }

        //update pheromone on each edge
        map<pair<pair<int,int>,pair<int,int>>, double> edge_pheromone_update;
        for(int i=0; i<ants; i++){
            vector<pair<pair<int,int>,pair<int,int>>> ant_path_edges;
            generate_edges(ant_path[i], ant_path_edges);
            int ant_path_perimeter = perimeter(ant_path_edges);

            //checking for better solutions (this is done here to use the already calculated perimeters)
            if(ant_path_perimeter < best_perimeter){
                best_perimeter = ant_path_perimeter;
                best_ant_path = ant_path[i];
                
                //this is totally optional altough it decreases max time spent but possibly giving up optimal solution 
                if(intersections(ant_path_edges) == 0){
                    max_iters = 0;
                    break;
                }
            }

            for(int j=0; j<(int)ant_path_edges.size(); j++){
                //one edge direction
                if(edge_pheromone_update.find(ant_path_edges[j]) == edge_pheromone_update.end()){
                    edge_pheromone_update[ant_path_edges[j]] = Q/ant_path_perimeter;
                }
                else{
                    edge_pheromone_update[ant_path_edges[j]] += Q/ant_path_perimeter;
                }
                //other edge direction
                pair<pair<int,int>,pair<int,int>> reversed_edge = make_pair(ant_path_edges[j].second,ant_path_edges[j].first);
                if(edge_pheromone_update.find(reversed_edge) == edge_pheromone_update.end()){
                    edge_pheromone_update[reversed_edge] = Q/ant_path_perimeter;
                }
                else{
                    edge_pheromone_update[reversed_edge] += Q/ant_path_perimeter;
                }
            }
        }

        //adding evaporation rate
        for(auto edge : edge_pheromone_update){
            edge.second += p * edge_pheromone[edge.first];
            
            //actually updating edge pheromome
            edge_pheromone[edge.first] = edge.second;
        }
        
        //resetting ant paths data structure
        for(int i=0; i<ants; i++){
            ant_path[i].clear();
        }

        max_iters--;
    }
    cout << "Ant Colony Optimization: ";
    show_vector_points(best_ant_path);
    return;
}

int main(){
    //setting seed based on system time
    srand(time(0));

    //input 
    int flag, n;
    cin >> n;
    cin >> flag;
    
    //vector/set containg all random generated points
    vector<pair<int,int>> vector_points;
    set<pair<int,int>> set_points;

    if(flag == 1){
        while((int) vector_points.size() != n){
            int x, y; 
            cin >> x >> y;
            pair<int,int> p = make_pair(x, y);
            vector_points.push_back(p);
            set_points.insert(p);
        }
        cout << endl;
    }
    else if(flag == 2){
        //1 - generating n points in range[-m..m] @@@@@@@@@@@@@
        int m;
        cin >> m;
        for(int i=0; i<n; i++){
            pair<int,int> p = make_pair((rand() % (m - (-m) + 1)) + (-m), (rand() % (m - (-m) + 1)) + (-m));
            while(set_points.find(p) != set_points.end()){
                p = make_pair((rand() % (m - (-m) + 1)) + (-m), (rand() % (m - (-m) + 1)) + (-m));
            }
            vector_points.push_back(p);
            set_points.insert(p);
        }
        cout << "Points: ";
        show_vector_points(vector_points);
        cout << endl;
    }
    
    //2a - generate points permutation @@@@@@@@@@@@@
    permutation(vector_points);

    //2b - nearest neighbour first heuristic @@@@@@@@@@@@@
    nearest_neighbour(n, set_points);

    //3 - "2-exchange" @@@@@@@@@@@@@
    //generating edges
    vector<pair<pair<int,int>,pair<int,int>>> vector_edges;
    generate_edges(vector_points, vector_edges);

    //generating neighbourhood by 2-exchange
    vector<vector<pair<pair<int,int>,pair<int,int>>>> two_exchange_neighbours;
    two_exchange(0, vector_edges, two_exchange_neighbours);

    //if theres no neighbours that means there were no intersections so we already reached the goal
    if((int)two_exchange_neighbours.size() == 0){
        cout << "Polygon already found. (as shown below)" << endl;
        show_vector_points(vector_points);
        return 0;
    }
    else{
        cout << "2-Exchange Neighbourhood:" << endl;
        //printing neighbourhood
        for(int i=0; i<(int)two_exchange_neighbours.size(); i++){
            show_vector_edges(two_exchange_neighbours[i]);
        }
    }
    cout << endl;

    //4 - Hill Climbing @@@@@@@@@@@@@
    cout << "Hill Climbing (based on different heuristics):" << endl;
    //4a - "Best-Improvement First" @@@@@@@@@@@@@
    hill_climbing(1, vector_edges);
    
    //4b - "First-Improvement" @@@@@@@@@@@@@
    hill_climbing(2, vector_edges);

    //4c - "Less Edge Intersections" @@@@@@@@@@@@@
    hill_climbing(3, vector_edges);

    //4d - Random Neighbour @@@@@@@@@@@@@
    hill_climbing(0, vector_edges);

    //5 - Simulated Annealing @@@@@@@@@@@@@
    simulated_annealing(vector_edges);

    //6 - Ant Colony Optimization Metaheuristic
    ant_colony(vector_points, set_points);

    return 0;
}