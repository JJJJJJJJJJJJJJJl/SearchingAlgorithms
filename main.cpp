#include <bits/stdc++.h>
using namespace std;

void show_vector_pairs(vector<pair<int,int>> v){
    int n = (int) v.size();
    for(int i=0; i<n; i++){
        cout << "(" << v[i].first << "," << v[i].second << ")";
        if(i != n-1){
            cout << " ";
        }
    }
    cout << endl;
    return;
}

void show_vector_edges(vector<pair<pair<int,int>,pair<int,int>>> v){
    int n = (int) v.size();
    for(int i=0; i<n; i++){
        cout << "[" << "(" << v[i].first.first << "," << v[i].first.second << ");(" << v[i].second.first << "," << v[i].second.second << ")" << "] ";
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

//here will do euclidean distance as (x1-x2)^2 + (y1-y2)^2 instead of the square root of it.
int euclidean_distance(pair<int,int> p1, pair<int,int> p2){
    return ((p1.first - p2.first) * (p1.first - p2.first)) + ((p1.second - p2.second) * (p1.second - p2.second));
}

//checks if two points are different
int points_different(pair<int,int> p1, pair<int,int> p2){
    return p1.first != p2.first || p1.second != p2.second;
}

//checks if p3 is within p1p2 rectangle
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

void generate_edges(vector<pair<int,int>> points, vector<pair<pair<int,int>,pair<int,int>>> &vector_edges){
    int n = (int) points.size();
    //generating edges from initial disposition
    vector_edges.push_back(make_pair(points[n-1], points[0]));
    for(int i=1; i<n; i++){
        pair<pair<int,int>,pair<int,int>> p = make_pair(points[i-1], points[i]);
        vector_edges.push_back(p);
    }
    return;
}

void two_exchange(vector<pair<pair<int,int>,pair<int,int>>> vector_edges, vector<vector<pair<pair<int,int>,pair<int,int>>>> &neighbourhood){
    int n = (int) vector_edges.size();
    set<pair<pair<pair<int,int>,pair<int,int>>,pair<pair<int,int>,pair<int,int>>>> vi;//visited intersections
    set<pair<pair<int,int>,pair<int,int>>> set_edges;//edges of original disposition (finding purposes)

    //filling set_edges up
    for(int i=0; i<n; i++){
        set_edges.insert(vector_edges[i]);
    }

    //generating neighbourhood by 2-exchange
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(vector_edges[i] != vector_edges[j]){
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
                    && set_edges.find(make_pair(vector_edges[j].second, vector_edges[j].first)) == set_edges.end()){
                        neighbourhood.push_back(vector_edges);
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
                    && set_edges.find(make_pair(vector_edges[j].second, vector_edges[j].first)) == set_edges.end()){
                        neighbourhood.push_back(vector_edges);
                    }
                    //reseting to original disposition
                    temp = vector_edges[i].second;
                    vector_edges[i].second = vector_edges[j].second;
                    vector_edges[j].second = temp;

                    vi.insert(make_pair(vector_edges[i], vector_edges[j]));
                    vi.insert(make_pair(vector_edges[j], vector_edges[i]));
                }
            }
        }
    }
    return;
}

int intersections_num(vector<pair<pair<int,int>,pair<int,int>>> v){
    int n = (int) v.size();
    int ans = 0;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            if(edges_intersect(v[i].first, v[i].second, v[j].first, v[j].second)){
                ans++;
            }
        }    
    }
    return ans;
}

int main(){
    //input 

    int flag, n;
    cout << "Number of points: ";
    cin >> n;
    cout << "Read from STDIN(1), Generate randomly(2): ";
    cin >> flag;
    cout << endl;
    
    //vector/set containg all random generated points
    vector<pair<int,int>> vector_pairs;
    set<pair<int,int>> set_pairs;

    if(flag == 1){
        while((int) vector_pairs.size() != n){
            int x, y; 
            cout << "PointCoordinates@ex('x y'): ";
            cin >> x >> y;
            pair<int,int> p = make_pair(x, y);
            vector_pairs.push_back(p);
            set_pairs.insert(p);
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
            while(set_pairs.find(p) != set_pairs.end()){
                p = make_pair((rand() % (m - (-m) + 1)) + (-m), (rand() % (m - (-m) + 1)) + (-m));
            }
            vector_pairs.push_back(p);
            set_pairs.insert(p);
        }
        cout << "Points: ";
        show_vector_pairs(vector_pairs);
        cout << endl;
    }
    
    //2a - generate points permutation (ty STL) @@@@@@@@@@@@@
    vector<pair<int,int>> permutation = vector_pairs;
    next_permutation(permutation.begin(), permutation.end());
    cout << "Permutation Solution: ";
    show_vector_pairs(permutation);
    cout << endl;

    //2b - nearest neighbour first heuristic @@@@@@@@@@@@@

    //by default initial node will always be the one with smallest x-value point, if draw, its ordered by y-value
    int visited[n];
    memset(visited, 0, sizeof(visited));
    vector<pair<int,int>> nnf_ans;
    set<pair<int,int>> help = set_pairs;

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
    show_vector_pairs(nnf_ans);
    cout << endl;

    //3 - "2-exchange" @@@@@@@@@@@@@
    vector<pair<pair<int,int>,pair<int,int>>> vector_edges;
    generate_edges(vector_pairs, vector_edges);
    vector<vector<pair<pair<int,int>,pair<int,int>>>> two_exchange_neighbours;
    two_exchange(vector_edges, two_exchange_neighbours);
    cout << "2-Exchange Neighbourhood: " << endl;
    show_state_neighbourhood(two_exchange_neighbours);
    cout << endl;

    //4d - @@@@@@@@@@@@@
    vector<vector<pair<pair<int,int>,pair<int,int>>>> temp = two_exchange_neighbours;
    int error = 0;
    int size = (int) temp.size();
    //chosing next neighbour randomly until it hits a simple polygon
    //not sure if should have some sort of breakpoint in case of infinite loops
    srand(time(0));
    vector<pair<pair<int,int>,pair<int,int>>> cur = temp[(rand() % ((size-1) - 0 + 1)) + 0];
    while(intersections_num(cur) != 0){
        temp.clear();
        two_exchange(cur, temp);
        if(temp.size() == 0){
            cout << "No legit neighbours were found (inconclusive)" << endl;
            error = 1;
            break;
        }
        cur = temp[(rand() % (((int)temp.size()-1) - 0 + 1)) + 0];
    }
    if(error == 0){
        cout << "d) ";
        show_vector_edges(cur);
    }
    return 0;
}