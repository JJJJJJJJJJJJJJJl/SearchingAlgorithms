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

//not the perimeter exactly since euclidean distance doesn't square root the squares sum so it avoids floating point approximations
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

/* //is it a simple polygon??
int simple_polygon(vector<pair<pair<int,int>,pair<int,int>>> edges){
    map<pair<int,int>,int> degrees;
    for(int i=0; i<(int)edges.size(); i++){
        if(degrees.find(edges[i].first) == degrees.end()){
            degrees[edges[i].first] = 1;
        }
        else degrees[edges[i].first]++;

        if(degrees.find(edges[i].second) == degrees.end()){
            degrees[edges[i].second] = 1;
        }
        else degrees[edges[i].second]++;
    }
    for(auto p : degrees){
        if(p.second != 2){
            return 0;
        }
    }
    if(intersections(edges) != 0){
        return 0;
    }
    return 1;
} */

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
//flag == 1 -> Generates all legit neighbours but only returns the one with lowest perimeter ("best-improvement first").
//flag == 2 -> Generates all legit neighbours but only returns the first it finds thats better than the current state ("first-improvement").
//flag == 3 -> Generates all legit neighbours but only returns the one with less edge intersections.
void two_exchange(int flag, vector<pair<pair<int,int>,pair<int,int>>> vector_edges, vector<vector<pair<pair<int,int>,pair<int,int>>>> &neighbourhood){
    int current_state_perimeter = perimeter(vector_edges);
    int lowest_perimeter = INT_MAX;//flag == 1 purposes, assuming perimeters wont go beyond INT_MAX (2147483647)
    int lowest_intersections = INT_MAX;//flag == 3 purposes, also assuming intersections wont go beyond INT_MAX (2147483647)
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

                        int cur_perimeter;//flag == 1 purposes
                        int cur_intersections;//flag == 3 purposes
                        if(flag == 1){
                            cur_perimeter = perimeter(vector_edges);
                            if(cur_perimeter < lowest_perimeter){
                                neighbourhood.clear();
                                neighbourhood.push_back(vector_edges);
                                lowest_perimeter = cur_perimeter;
                            }
                        }
                        else if(flag == 3){
                            cur_intersections = intersections(vector_edges);
                            if(cur_intersections < lowest_intersections){
                                neighbourhood.clear();
                                neighbourhood.push_back(vector_edges);
                                lowest_intersections = cur_intersections;
                            }
                        }
                        else neighbourhood.push_back(vector_edges);
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
                    //not so sure how this case should be reversed, couldnt find a single test case where
                    //this one didnt disconnect the path, so here's probably a bug. 

                    temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].second;
                    vector_edges[j].second = temp;
                    //if new edges dont already exist
                    if(set_edges.find(vector_edges[i]) == set_edges.end() 
                    && set_edges.find(make_pair(vector_edges[i].second, vector_edges[i].first)) == set_edges.end()
                    && set_edges.find(vector_edges[j]) == set_edges.end()
                    && set_edges.find(make_pair(vector_edges[j].second, vector_edges[j].first)) == set_edges.end()
                    && is_it_connected(1, vector_edges)){
                        int cur_perimeter;//flag == 1 purposes
                        int cur_intersections;//flag == 3 purposes
                        if(flag == 1){
                            cur_perimeter = perimeter(vector_edges);
                            if(cur_perimeter < lowest_perimeter){
                                neighbourhood.clear();
                                neighbourhood.push_back(vector_edges);
                                lowest_perimeter = cur_perimeter;
                            }
                        }
                        else if(flag == 3){
                            cur_intersections = intersections(vector_edges);
                            if(cur_intersections < lowest_intersections){
                                neighbourhood.clear();
                                neighbourhood.push_back(vector_edges);
                                lowest_intersections = cur_intersections;
                            }
                        }
                        else neighbourhood.push_back(vector_edges);
                    }
                    //reseting to original disposition
                    temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].second;
                    vector_edges[j].second = temp;

                    //first legit neighbour was found
                    if(flag == 2){
                        if((int) neighbourhood.size() > 0 && perimeter(neighbourhood[0]) < current_state_perimeter){
                            return;
                        }
                        else{
                            neighbourhood.clear();
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

void hill_climbing(int flag, vector<pair<pair<int,int>,pair<int,int>>> vector_edges){
    srand(time(0));
    vector<vector<pair<pair<int,int>,pair<int,int>>>> temp; 
    vector<pair<pair<int,int>,pair<int,int>>> cur;
    vector<pair<pair<int,int>,pair<int,int>>> last;

    two_exchange(flag, vector_edges, temp);
    if(flag == 1 || flag == 2 || flag == 3){
        cur = temp[0];    
    }
    else{
        cur = temp[(rand() % (((int)temp.size()-1) - 0 + 1)) + 0];
    }
    last = cur;

    while(perimeter(cur) <= perimeter(last)){
        temp.clear();
        two_exchange(flag, cur, temp);
        last = cur;
        if((int) temp.size() > 0){
            if(flag == 1 || flag == 2 || flag == 3){
                cur = temp[0];
            }
            else{
                cur = temp[(rand() % (((int)temp.size()-1) - 0 + 1)) + 0];
            }
        }
        else{
            break;
        }
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
    int diff = chosen_neighbour_energy - current_state_energy;
    srand(time(0));
    double metropolis = (rand() / double(RAND_MAX)); 
    if(diff < 0 || metropolis < exp(-diff / T)){
        return 1;
    }
    return 0;
}

//energy meaning number of intersections
void simulated_annealing(vector<pair<pair<int,int>,pair<int,int>>> initial){
    srand(time(0));
    vector<vector<pair<pair<int,int>,pair<int,int>>>> current_candidates;
    vector<vector<pair<pair<int,int>,pair<int,int>>>> new_candidates;
    vector<pair<pair<int,int>,pair<int,int>>> cur;
    vector<pair<pair<int,int>,pair<int,int>>> last;
    vector<pair<pair<int,int>,pair<int,int>>> best_sa;
    int best_sa_perimeter;

    last = initial;
    best_sa_perimeter = perimeter(initial);

    //constants
    double T = 500;
    double cooling_rate = 0.95;
    int max_steps = 100;

    int cur_step = 0;
    while(cur_step != max_steps && T > 0){
        cout << "step: " << cur_step << endl;
        new_candidates.clear();
        if(cur_step != 0){
            two_exchange(0, cur, new_candidates);
        }
        else{
            two_exchange(0, initial, new_candidates);
        }
        for(int i=0; i<(int)new_candidates.size(); i++){
            current_candidates.push_back(new_candidates[i]);
        }
        if((int) current_candidates.size() > 0){
            int rand_index = (rand() % (((int)current_candidates.size()-1) + 1));
            cur = current_candidates[rand_index];
            int cur_energy = intersections(cur);
            int last_energy = intersections(last);
            //if you deny a candidate, should it be removed from possible candidates?
            while(1/* !P(cur_energy, last_energy, T) */){
                rand_index = (rand() % (((int)current_candidates.size()-1) + 1));
                cur = current_candidates[rand_index];
                cur_energy = intersections(cur);
                if(P(cur_energy, last_energy, T)){
                    break;
                }
                else{
                    current_candidates.erase(current_candidates.begin() + rand_index);
                }
            }
            int cur_perimeter = perimeter(cur);
            if(cur_perimeter < best_sa_perimeter){
                best_sa_perimeter = cur_perimeter;
                best_sa = cur;
            }
            current_candidates.erase(current_candidates.begin() + rand_index);
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
}

void permutation(vector<pair<int,int>> vector_points){
    vector<pair<int,int>> permutation = vector_points;
    next_permutation(permutation.begin(), permutation.end());
    cout << "Permutation Solution: ";
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
    cout << "Nearest Neighbour Solution: ";
    show_vector_points(nnf_ans);
    cout << endl;
    return;
}

void ant_colony(vector<pair<int,int>> points){
    int n = (int) points.size();
    map<pair<pair<int,int>,pair<int,int>>,int> edges;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(i != j){
                edges[make_pair(points[i], points[j])] = 0;
            }
        }
    }

    //int ants = 10;

    return;
}

int main(){

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
        srand(time(0));
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
    cout << "intersections: " << intersections(vector_edges) << endl;
    //generating neighbourhood by 2-exchange
    vector<vector<pair<pair<int,int>,pair<int,int>>>> two_exchange_neighbours;
    two_exchange(0, vector_edges, two_exchange_neighbours);

    cout << "2-Exchange Neighbourhood:" << endl;
    //if theres no neighbours that means there were no intersections so we already reached the goal
    if((int)two_exchange_neighbours.size() == 0){
        cout << "Polygon already found. (as shown below)" << endl;
        show_vector_points(vector_points);
    }
    else{
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
    ant_colony(vector_points);

    return 0;
}


//A B C D E F

//Exchange case 1, AB CD -> AC BD 
//[(1,1),(3,3)] [(3,3),(5,0)] [(5,0),(2,-1)] [(2,-1),(3,1)] [(3,1),(1,3)] [(1,3),(1,1)]
//------------                                              -------------
//[(1,1),(3,1)] [(3,3),(5,0)] [(5,0),(2,-1)] [(2,-1),(3,1)] [(3,3),(1,3)] [(1,3),(1,1)]
//        **                                                  **
//[(1,1),(3,1)] [(3,3),(5,0)] [(5,0),(2,-1)] [(2,-1),(3,1)] [(3,3),(1,3)] [(1,3),(1,1)]   
//             [(2,-1),(3,1)] [(5,0),(2,-1)] [(3,3),(5,0)]
//             [(3,1),(2,-1)] [(2,-1),(5,0)] [(5,0),(3,3)]
//FINAL:
//[(1,1),(3,1)] [(3,1),(2,-1)] [(2,-1),(5,0)] [(5,0),(3,3)] [(3,3),(1,3)] [(1,3),(1,1)]


//Exchange case 2, AB CD -> AD CB
//No test cases found, unknown...404