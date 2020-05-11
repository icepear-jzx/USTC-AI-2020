#include <cstdlib>
#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>
#include <climits>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
 
using namespace std;

enum direction {UP, RIGHT, DOWN, LEFT};

unordered_map<string, int> visited_states;

int goal_state[5][5] = {
    {1, 2, 3, 4, 5},
    {7, 7, 8, 9, 10},
    {6, 7, 11, 12, 13},
    {14, 15, 16, 17, 18},
    {19, 20, 21, 0, 0}
};

int cnt = 0;

int goal_pos_i[22] = {4, 0, 0, 0, 0, 0, 2, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4};
int goal_pos_j[22] = {4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2};

class Node {
    public:
        int state[5][5];
        string state_str = "";
        int pos_i_0s[2] = {-1, -1};
        int pos_j_0s[2] = {-1, -1};
        Node* parent = NULL;
        vector<Node*> children;
        vector<int> moves_i;
        vector<int> moves_j;
        vector<direction> moves_d;
        int from_n;
        direction from_d;
        int f_score = 0;
        int g_score = 0;
        int h_score = 0;

        Node(int cur_state[5][5], Node* cur) {
            parent = cur;
            for (int i=0; i < 5; i++) {
                for (int j=0; j < 5; j++) {
                    state[i][j] = cur_state[i][j];
                }
            }
            update();
        }

        ~Node() {
            int n = children.size();
            for (int i=0; i < n; i++) {
                delete children[i];
            }
        }

        void show_state() {
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; j++) {
                    cout << state[i][j] << '\t';
                }
                cout << endl;
            }
            cout << endl;
        }

        void show_moves() {
            int n = moves_i.size();
            for (int i=0; i < n; i++) {
                cout << moves_i[i] << ' ' << moves_j[i] << ' ' << moves_d[i] << endl;
            }
        }

        void show_children_state() {
            int n = children.size();
            for (int k=0; k < n; k++) {
                for (int i = 0; i < 5; ++i) {
                    for (int j = 0; j < 5; j++) {
                        cout << children[k]->state[i][j] << '\t';
                    }
                    cout << endl;
                }
                cout << endl;
            }
        }

        void expend_nodes() {
            setup_moves();
            setup_children();
        }

        void move(int pos_i, int pos_j, direction direct) {
            assert(("Can't move 0.", state[pos_i][pos_j] != 0));
            if (state[pos_i][pos_j] == 7) {
                assert(("Pos should be the center of 7.", 
                    state[pos_i][pos_j-1] == 7 && state[pos_i+1][pos_j] == 7));
                switch (direct) {
                    case UP: {
                        assert(("Can't move.", state[pos_i-1][pos_j] == 0 && state[pos_i-1][pos_j-1] == 0));
                        state[pos_i-1][pos_j] = state[pos_i][pos_j];
                        state[pos_i-1][pos_j-1] = state[pos_i][pos_j];
                        state[pos_i][pos_j-1] = 0;
                        state[pos_i+1][pos_j] = 0;
                        break;
                    }
                    case RIGHT: {
                        assert(("Can't move.", state[pos_i][pos_j+1] == 0 && state[pos_i+1][pos_j+1] == 0));
                        state[pos_i][pos_j+1] = state[pos_i][pos_j];
                        state[pos_i+1][pos_j+1] = state[pos_i][pos_j];
                        state[pos_i][pos_j-1] = 0;
                        state[pos_i+1][pos_j] = 0;
                        break;
                    }
                    case DOWN: {
                        assert(("Can't move.", state[pos_i+2][pos_j] == 0 && state[pos_i+1][pos_j-1] == 0));
                        state[pos_i+2][pos_j] = state[pos_i][pos_j];
                        state[pos_i+1][pos_j-1] = state[pos_i][pos_j];
                        state[pos_i][pos_j] = 0;
                        state[pos_i][pos_j-1] = 0;
                        break;
                    }
                    case LEFT: {
                        assert(("Can't move.", state[pos_i][pos_j-2] == 0 && state[pos_i+1][pos_j-1] == 0));
                        state[pos_i][pos_j-2] = state[pos_i][pos_j];
                        state[pos_i+1][pos_j-1] = state[pos_i][pos_j];
                        state[pos_i][pos_j] = 0;
                        state[pos_i+1][pos_j] = 0;
                        break;
                    }
                }
            } else {
                switch (direct) {
                    case UP: {
                        assert(("Can't move.", state[pos_i-1][pos_j] == 0));
                        state[pos_i-1][pos_j] = state[pos_i][pos_j];
                        state[pos_i][pos_j] = 0;
                        break;
                    }
                    case RIGHT: {
                        assert(("Can't move.", state[pos_i][pos_j+1] == 0));
                        state[pos_i][pos_j+1] = state[pos_i][pos_j];
                        state[pos_i][pos_j] = 0;
                        break;
                    }
                    case DOWN: {
                        assert(("Can't move.", state[pos_i+1][pos_j] == 0));
                        state[pos_i+1][pos_j] = state[pos_i][pos_j];
                        state[pos_i][pos_j] = 0;
                        break;
                    }
                    case LEFT: {
                        assert(("Can't move.", state[pos_i][pos_j-1] == 0));
                        state[pos_i][pos_j-1] = state[pos_i][pos_j];
                        state[pos_i][pos_j] = 0;
                        break;
                    }
                }
            }
            from_n = parent->state[pos_i][pos_j];
            from_d = direct;
            update();
        }

        bool is_goal() {
            return h_score == 0;
        }

    private:
        void center_of(int pos_i, int pos_j, int& center_i, int& center_j) {
            if (pos_i < 0 || pos_i >= 5 || pos_j < 0 || pos_j >=5) {
                center_i = -1;
                center_j = -1;
            } else {
                if (state[pos_i][pos_j] == 7) {
                    if (pos_j > 0 && state[pos_i][pos_j-1] == 7) {
                        center_i = pos_i;
                        center_j = pos_j;
                    } else if (pos_i > 0 && state[pos_i-1][pos_j] == 7) {
                        center_i = pos_i - 1;
                        center_j = pos_j;
                    } else {
                        center_i = pos_i;
                        center_j = pos_j + 1;
                    }
                } else {
                    center_i = pos_i;
                    center_j = pos_j;
                }
            }
        }

        bool move_is_available(int pos_i, int pos_j, direction direct) {
            if (pos_i < 0 || pos_i >= 5 || pos_j < 0 || pos_j >=5) {
                return false;
            }
            if (state[pos_i][pos_j] == 0) {
                return false;
            }
            if (state[pos_i][pos_j] == 7) {
                assert(("Pos should be the center of 7.", 
                    state[pos_i][pos_j-1] == 7 && state[pos_i+1][pos_j] == 7));
                switch (direct) {
                    case UP: {
                        if (pos_i > 0 && pos_j > 0 && state[pos_i-1][pos_j] == 0 &&
                                state[pos_i-1][pos_j-1] == 0){
                            return true;
                        } else {
                            return false;
                        }
                    }
                    case RIGHT: {
                        if (pos_i < 4 && pos_j < 4 && state[pos_i][pos_j+1] == 0 &&
                                state[pos_i+1][pos_j+1] == 0){
                            return true;
                        } else {
                            return false;
                        }
                    }
                    case DOWN: {
                        if (pos_i < 3 && pos_j > 0 && state[pos_i+2][pos_j] == 0 &&
                                state[pos_i+1][pos_j-1] == 0){
                            return true;
                        } else {
                            return false;
                        }
                    }
                    case LEFT: {
                        if (pos_j > 1 && pos_i < 4 && state[pos_i][pos_j-2] == 0 &&
                                state[pos_i+1][pos_j-1] == 0){
                            return true;
                        } else {
                            return false;
                        }
                    }
                }
            } else {
                switch (direct) {
                    case UP: {
                        if (pos_i > 0 && state[pos_i-1][pos_j] == 0){
                            return true;
                        } else {
                            return false;
                        }
                    }
                    case RIGHT: {
                        if (pos_j < 4 && state[pos_i][pos_j+1] == 0){
                            return true;
                        } else {
                            return false;
                        }
                    }
                    case DOWN: {
                        if (pos_i < 4 && state[pos_i+1][pos_j] == 0){
                            return true;
                        } else {
                            return false;
                        }
                    }
                    case LEFT: {
                        if (pos_j > 0 && state[pos_i][pos_j-1] == 0){
                            return true;
                        } else {
                            return false;
                        }
                    }
                }
            }
        }

        bool move_exists(int pos_i, int pos_j, direction direct) {
            int n = moves_i.size();
            for (int i=0; i < n; i++) {
                if (moves_i[i] == pos_i && moves_j[i] == pos_j && moves_d[i] == direct) {
                    return true;
                }
            }
            return false;
        }

        void add_move(int pos_i, int pos_j, direction direct) {
            moves_i.push_back(pos_i);
            moves_j.push_back(pos_j);
            moves_d.push_back(direct);
        }

        void setup_moves() {
            for (int i=0; i < 2; i++) {
                int pos_i_0 = pos_i_0s[i];
                int pos_j_0 = pos_j_0s[i];
                int center_i, center_j;
                center_of(pos_i_0-1, pos_j_0, center_i, center_j);
                if (move_is_available(center_i, center_j, DOWN) &&
                        !move_exists(center_i, center_j, DOWN)) {
                    add_move(center_i, center_j, DOWN);
                }
                center_of(pos_i_0+1, pos_j_0, center_i, center_j);
                if (move_is_available(center_i, center_j, UP) && 
                        !move_exists(center_i, center_j, UP)) {
                    add_move(center_i, center_j, UP);
                }
                center_of(pos_i_0, pos_j_0-1, center_i, center_j);
                if (move_is_available(center_i, center_j, RIGHT) &&
                        !move_exists(center_i, center_j, RIGHT)) {
                    add_move(center_i, center_j, RIGHT);
                }
                center_of(pos_i_0, pos_j_0+1, center_i, center_j);
                if (move_is_available(center_i, center_j, LEFT) && 
                        !move_exists(center_i, center_j, LEFT)) {
                    add_move(center_i, center_j, LEFT);
                }
            }
        }

        void setup_children() {
            int n = moves_i.size();
            for (int i=0; i < n; i++) {
                int pos_i = moves_i[i];
                int pos_j = moves_j[i];
                direction direct = moves_d[i];
                Node* new_node = new Node(state, this);
                new_node->move(pos_i, pos_j, direct);
                children.push_back(new_node);
            }
        }

        void update() {
            update_pos0();
            update_state_str();
            update_score();
        }

        void update_pos0() {
            pos_i_0s[0] = -1;
            pos_i_0s[1] = -1;
            pos_j_0s[0] = -1;
            pos_j_0s[1] = -1;
            for (int i=0; i < 5; i++) {
                for (int j=0; j < 5; j++) {
                    if (state[i][j] == 0) {
                        if (pos_i_0s[0] == -1) {
                            pos_i_0s[0] = i;
                            pos_j_0s[0] = j;
                        } else {
                            pos_i_0s[1] = i;
                            pos_j_0s[1] = j;
                        }
                    }
                }
            }
        }

        void update_state_str() {
            state_str = "";
            for (int i=0; i < 5; i++) {
                for (int j=0; j < 5; j++) {
                    state_str += to_string(state[i][j]);
                    state_str += ' ';
                }
            }
        }

        void update_score() {
            if (parent) {
                g_score = parent->g_score + 1;
            } else {
                g_score = 0;
            }
            h_score = h1();
            f_score = g_score + h_score;
            // cout << g_score << ' ' << h_score << ' ' << f_score << endl;
        }

        int h1() {
            int h_score = 0;
            for (int i=0; i < 5; i++) {
                for (int j=0; j < 5; j++) {
                    if (state[i][j] == 0) {
                        continue;
                    } else {
                        int center_i, center_j;
                        center_of(i, j, center_i, center_j);
                        if (i == center_i && j == center_j) {
                            int dist = abs(i - goal_pos_i[state[i][j]]) + 
                                abs(j - goal_pos_j[state[i][j]]);
                            h_score += dist;
                        }
                    }
                }
            }
            return h_score;
        }

        int h2() {
            int h_score = 0;
            for (int i=0; i < 5; i++) {
                for (int j=0; j < 5; j++) {
                    if (state[i][j] == 0) {
                        continue;
                    } else {
                        int center_i, center_j;
                        center_of(i, j, center_i, center_j);
                        if (i == center_i && j == center_j) {
                            int dist = abs(i - goal_pos_i[state[i][j]]) + 
                                abs(j - goal_pos_j[state[i][j]]);
                            if (state[i][j] == 7) {
                                if (dist <= 1) {
                                    h_score += dist;
                                } else {
                                    h_score += 5*(dist-1) + 1;
                                }
                            } else {
                                if (dist <= 2) {
                                    h_score += dist;
                                } else {
                                    h_score += 3*(dist-2) + 2;
                                }
                            }
                        }
                    }
                }
            }
            return h_score;
        }
};

struct CompareNode : public std::binary_function<Node*, Node*, bool>                                                                                     
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        // if (lhs->f_score == rhs->f_score) {
        //     return lhs->g_score < rhs->g_score;
        // } else {
        //     return lhs->f_score > rhs->f_score;
        // }
        return lhs->f_score > rhs->f_score;
    }
};

Node* astar_search(int init_state[5][5]) {
    visited_states.clear();
    priority_queue<Node *, vector<Node *>, CompareNode > edge_nodes;
    Node* node0 = new Node(init_state, NULL);
    Node* node;
    cnt = 0;
    edge_nodes.push(node0);
    cnt += 1;
    while (!edge_nodes.empty()) {
        node = edge_nodes.top();
        edge_nodes.pop();
        // cout << node->g_score << ' ' << node->h_score << ' ' << node->f_score << endl;
        if (node->is_goal()) {
            break;
        } else {
            node->expend_nodes();
            int n = node->children.size();
            for (int i=0; i < n; i++) {
                Node *child = node->children[i];
                if (visited_states.find(child->state_str) == visited_states.end() ||
                        visited_states[child->state_str] > child->f_score) {
                    visited_states[child->state_str] = child->f_score;
                    edge_nodes.push(child);
                    cnt += 1;
                }
            }
        }
    }
    return node;
}

Node* idastar_search(int init_state[5][5]) {
    int f_limit = 0;
    Node* node;
    Node* node0 = new Node(init_state, NULL);
    cnt = 0;
    f_limit = node0->f_score;
    while (true) {
        int next_f_limit = INT_MAX;
        visited_states.clear();
        priority_queue<Node *, vector<Node *>, CompareNode > edge_nodes;
        edge_nodes.push(node0);
        cnt += 1;
        cout << "f_limit: " << f_limit << endl;
        while (!edge_nodes.empty()) {
            node = edge_nodes.top();
            edge_nodes.pop();
            // cout << node->g_score << ' ' << node->h_score << ' ' << node->f_score << endl;
            if (node->f_score > f_limit) {
                next_f_limit = (next_f_limit > node->f_score)? node->f_score : next_f_limit;
            } else {
                if (node->is_goal()) {
                    return node;
                } else {
                    if (node->children.size() == 0) {
                        node->expend_nodes();
                    }
                    int n = node->children.size();
                    for (int i=0; i < n; i++) {
                        Node *child = node->children[i];
                        if (visited_states.find(child->state_str) == visited_states.end() ||
                                visited_states[child->state_str] > child->f_score) {
                            visited_states[child->state_str] = child->f_score;
                            edge_nodes.push(child);
                            cnt += 1;
                        }
                    }
                }
            }
        }
        f_limit = next_f_limit;
        // delete node0;
        // node0 = new Node(init_state, NULL);
    }
    return NULL;
}

void input_state(int init_state[5][5], FILE *fin) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if(j == 4)
                fscanf(fin, "%d", &init_state[i][j]);
            else
                fscanf(fin, "%d,", &init_state[i][j]);
        }
    }
    return;
}

void print_path(FILE *fout, Node* node) {
    stack<int> moves_n;
    stack<direction> moves_d;
    while(node->parent) {
        moves_n.push(node->from_n);
        moves_d.push(node->from_d);
        node = node->parent;
    }
    int n;
    direction d;
    char direct_map[4] = {'u', 'r', 'd', 'l'};
    while(!moves_n.empty()) {
        n = moves_n.top();
        moves_n.pop();
        d = moves_d.top();
        moves_d.pop();
        if(moves_n.empty())
            fprintf(fout, "(%d,%c)\n", n, direct_map[d]);
        else
            fprintf(fout, "(%d,%c); ", n, direct_map[d]);
    }
    return;
}

int main() {
    int init_state[5][5];
    char input_dir[30] = "../input/";
    char output_dir[30] = "../output/";
    char filename[20];
    printf("Input filename：\n");
    scanf("%s", filename);
    strcat(input_dir, filename);
    strcat(output_dir, filename);

    FILE *fin, *fout;
    fin = fopen(input_dir, "r");
    if (fin == NULL) {
        printf("Error: cannot open %s!\n", input_dir);
        return -1;
    }
    fout = fopen(output_dir, "w");
    if (fout == NULL)
    {
        printf("Error: cannot open %s!\n", output_dir);
        return -1;
    }

    input_state(init_state, fin);

    Node* node;
    node = astar_search(init_state);
    cout << cnt << endl;
    cout << node->f_score << endl;
    print_path(fout, node);
    delete node;
    fclose(fin);
    fclose(fout);

    return 0;
}
