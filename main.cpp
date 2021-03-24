#include <bits/stdc++.h>
using namespace std;

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

void show_set(set<pair<pair<int,int>,pair<int,int>>> set){
    for(pair<pair<int,int>,pair<int,int>> edge : set){
        cout << "[" << "(" << edge.first.first << "," << edge.first.second << ");(" << edge.second.first << "," << edge.second.second << ")" << "] ";
    }
    cout << endl;
    return;
}

void show_state_neighbourhood(vector<vector<pair<pair<int,int>,pair<int,int>>>> v){
    for(int i=0; i<(int)v.size(); i++){
        for(int j=0; j<(int)v[i].size(); j++){
            cout << "[" << "(" << v[i][j].first.first << "," << v[i][j].first.second << ");(" << v[i][j].second.first << "," << v[i][j].second.second << ")" << "] ";
        }   
        cout << endl;
    }
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

//not the perimeter exactly since euclidean distance doesn't square root the squares sum
int perimeter(vector<pair<pair<int,int>,pair<int,int>>> edges){
    int ans = 0;
    for(int i=0; i<(int)edges.size(); i++){
        ans += euclidean_distance(edges[i].first, edges[i].second);
    }
    return ans;
}

//number of intersections
int intersections(vector<pair<pair<int,int>,pair<int,int>>> edges){
    int ans = 0;
    for(int i=0; i<(int)edges.size(); i++){
        for(int j=0; j<(int)edges.size(); j++){
            if(edges_intersect(edges[i].first, edges[i].second, edges[j].first, edges[j].second)){
                ans++;
            }
        }
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
}

int scalar_product(pair<pair<int,int>,pair<int,int>> a, pair<pair<int,int>,pair<int,int>> b){
    pair<int,int> a_vector = make_pair(a.second.first - a.first.first, a.second.second - a.first.second);
    pair<int,int> b_vector = make_pair(b.second.first - b.first.first, b.second.second - b.first.second);
    return ((a_vector.first * b_vector.first) + (a_vector.second * b_vector.second));
}

int collinear_opposite_direction_edges(pair<pair<int,int>,pair<int,int>> a, pair<pair<int,int>,pair<int,int>> b){
    double a_slope;
    double b_slope;

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

    //edges are not collinear
    if(a_slope != b_slope){
        return 0;
    }

    //edges have the same direction
    if(scalar_product(a, b) > 0){
        return 0;
    }

    //edges are collinear and have opposite directions
    return 1;
}

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
//flag == 2, also prints it
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

    //creating graph               
    vector<int> graph[index+1];
    for(int i=0; i<(int)edges.size(); i++){
        graph[point_index[edges[i].first]].push_back(point_index[edges[i].second]);
        graph[point_index[edges[i].second]].push_back(point_index[edges[i].first]);
    }

    //dfs through point 0
    //then checking visited array, if all were visited(1) then is it connected
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

void show_vector_edges(vector<pair<pair<int,int>,pair<int,int>>> edges){
    is_it_connected(2, edges);
    cout << endl;
    return;
}

//flag == 0 -> Generates all legit neighbours.
//flag == 1 -> Generates all legit neighbours but only returns the one with lowest perimeter ("best-improvement first")
//flag == 2 -> Generates first legit neighbour it finds and stops. ("first-improvement")
//flag == 3 -> Genereates all legit neighbours but only returns the one with less edge intersections.
//flag == 4 -> Generates all legit neighbours since we choosing one randomly.
void two_exchange(int flag, vector<pair<pair<int,int>,pair<int,int>>> vector_edges, vector<vector<pair<pair<int,int>,pair<int,int>>>> &neighbourhood){
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
                if(collinear_opposite_direction_edges(vector_edges[i], vector_edges[j])){
                    continue;
                }
                //cout <<"BBB"<<endl;
                if(edges_intersect(vector_edges[i].first, vector_edges[i].second, vector_edges[j].first, vector_edges[j].second) &&
                vi.find(make_pair(vector_edges[i], vector_edges[j])) == vi.end() && vi.find(make_pair(vector_edges[j], vector_edges[i])) == vi.end()){
                    //{A,B} {C,D} -> {A,C} {B,D} 
                    pair<int,int> temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].first;
                    vector_edges[j].first = temp;
                    //if new edges dont already exist
                    if(set_edges.find(vector_edges[i]) == set_edges.end() 
                    && set_edges.find(make_pair(vector_edges[i].second, vector_edges[i].first)) == set_edges.end()
                    && set_edges.find(vector_edges[j]) == set_edges.end()
                    && set_edges.find(make_pair(vector_edges[j].second, vector_edges[j].first)) == set_edges.end()
                    && is_it_connected(1, vector_edges)){
                        int cur_perimeter = perimeter(vector_edges);//flag == 1 purposes
                        int cur_intersections = intersections(vector_edges);//flag == 3 purposes
                        if(flag == 1){
                            if(cur_perimeter < lowest_perimeter){
                                neighbourhood.clear();
                                neighbourhood.push_back(vector_edges);
                                lowest_perimeter = cur_perimeter;
                            }
                        }
                        else if(flag == 3){
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

                    //{A,B} {C,D} -> {A,D} {C,B}
                    temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].second;
                    vector_edges[j].second = temp;
                    //if new edges dont already exist
                    if(set_edges.find(vector_edges[i]) == set_edges.end() 
                    && set_edges.find(make_pair(vector_edges[i].second, vector_edges[i].first)) == set_edges.end()
                    && set_edges.find(vector_edges[j]) == set_edges.end()
                    && set_edges.find(make_pair(vector_edges[j].second, vector_edges[j].first)) == set_edges.end()
                    && is_it_connected(1, vector_edges)){
                        int cur_perimeter = perimeter(vector_edges);//flag == 1 purposes
                        int cur_intersections = intersections(vector_edges);//flag == 3 purposes
                        if(flag == 1){
                            if(cur_perimeter < lowest_perimeter){
                                neighbourhood.clear();
                                neighbourhood.push_back(vector_edges);
                                lowest_perimeter = cur_perimeter;
                            }
                        }
                        else if(flag == 3){
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
                    if(flag == 2 && (int) neighbourhood.size() > 0){
                        return;
                    }

                    vi.insert(make_pair(vector_edges[i], vector_edges[j]));
                    vi.insert(make_pair(vector_edges[j], vector_edges[i]));
                }
            }
        }
    }
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
        cout << "Range (x -> [-x..x]): ";
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
    
    //2a - generate points permutation (ty STL) @@@@@@@@@@@@@
    vector<pair<int,int>> permutation = vector_points;
    next_permutation(permutation.begin(), permutation.end());
    cout << "Permutation Solution: ";
    show_vector_points(permutation);
    permutation.clear();
    cout << endl;

    //2b - nearest neighbour first heuristic @@@@@@@@@@@@@

    //by default initial node will always be the one with smallest x-value point, if draw, its ordered by y-value
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
    nnf_ans.clear();
    help.clear();
    cout << endl;

    //3 - "2-exchange" @@@@@@@@@@@@@
    vector<pair<pair<int,int>,pair<int,int>>> vector_edges;
    generate_edges(vector_points, vector_edges);
    vector<vector<pair<pair<int,int>,pair<int,int>>>> two_exchange_neighbours;
    two_exchange(0, vector_edges, two_exchange_neighbours);
    cout << "2-Exchange Neighbourhood:" << endl;
    //show_state_neighbourhood(two_exchange_neighbours);
    for(int i=0; i<(int)two_exchange_neighbours.size(); i++){
        show_vector_edges(two_exchange_neighbours[i]);
    }
    cout << endl;

    //4 - Hill Climbing @@@@@@@@@@@@@
    cout << "Hill Climbing (based on different heuristics):" << endl;
    //4a - "Best-Improvement First" @@@@@@@@@@@@@
    vector<vector<pair<pair<int,int>,pair<int,int>>>> temp_a;
    two_exchange(1, vector_edges, temp_a);
    vector<pair<pair<int,int>,pair<int,int>>> cur_a = temp_a[0];
    vector<pair<pair<int,int>,pair<int,int>>> last_a;
    while(!simple_polygon(cur_a) && (int) temp_a.size() > 0){
        temp_a.clear();
        two_exchange(1, cur_a, temp_a);
        last_a = cur_a;
        if((int) temp_a.size() > 0) cur_a = temp_a[0];
        //it breaks when candidate's worse than current state
        if(perimeter(cur_a) > perimeter(last_a)){
            break;
        }
    }
    cout << "Best-Improvement First: ";
    show_vector_edges(cur_a);
    cout << endl;
    cur_a.clear();
    last_a.clear();
    temp_a.clear();

    //4b - "First-Improvement" @@@@@@@@@@@@@
    vector<vector<pair<pair<int,int>,pair<int,int>>>> temp_b = two_exchange_neighbours;
    vector<pair<pair<int,int>,pair<int,int>>> cur_b = temp_b[0];
    vector<pair<pair<int,int>,pair<int,int>>> last_b;
    while(!simple_polygon(cur_b) && (int) temp_b.size() > 0){
        temp_b.clear();
        two_exchange(2, cur_b, temp_b);
        last_b = cur_b;
        if((int) temp_b.size() > 0) cur_b = temp_b[0];
        //it breaks when candidate's worse than current state
        if(perimeter(cur_b) > perimeter(last_b)){
            break;
        }
    }
    cout << "First-Improvement: ";
    show_vector_edges(cur_b);
    cout << endl;
    cur_b.clear();
    last_b.clear();
    temp_b.clear();

    //4c - "Less Edge Intersections" @@@@@@@@@@@@@
    vector<vector<pair<pair<int,int>,pair<int,int>>>> temp_c;
    two_exchange(3, vector_edges, temp_c);
    vector<pair<pair<int,int>,pair<int,int>>> cur_c = temp_c[0];
    vector<pair<pair<int,int>,pair<int,int>>> last_c;
    while(!simple_polygon(cur_c) && (int) temp_c.size() > 0){
        temp_c.clear();
        two_exchange(3, cur_c, temp_c);
        last_c = cur_c;
        if((int) temp_c.size() > 0) cur_c = temp_c[0];
        //it breaks when candidate's worse than current state
        if(perimeter(cur_c) > perimeter(last_c)){
            break;
        }
    }
    cout << "Less Edge Intersections: ";
    show_vector_edges(cur_c);
    cout << endl;
    cur_c.clear();
    last_c.clear();
    temp_c.clear();

    //4d - Random Neighbour @@@@@@@@@@@@@
    vector<vector<pair<pair<int,int>,pair<int,int>>>> temp_d = two_exchange_neighbours;
    //chosing next neighbour randomly until it hits a simple polygon
    //not sure if should have some sort of breakpoint in case of infinite loops
    srand(time(0));
    vector<pair<pair<int,int>,pair<int,int>>> cur_d = temp_d[(rand() % (((int)temp_d.size()-1) - 0 + 1)) + 0];
    vector<pair<pair<int,int>,pair<int,int>>> last_d;
    while(!simple_polygon(cur_d) && (int) temp_d.size() > 0){
        temp_d.clear();
        two_exchange(4, cur_d, temp_d);
        last_d = cur_d;
        if((int) temp_d.size() > 0) cur_d = temp_d[(rand() % (((int)temp_d.size()-1) - 0 + 1)) + 0];
        //it breaks when candidate's worse than current state
        if(perimeter(cur_d) > perimeter(last_d)){
            break;
        }
    }
    cout << "Random Neighbour: ";
    show_vector_edges(cur_d);
    cur_d.clear();
    last_d.clear();
    temp_d.clear();

    return 0;
}