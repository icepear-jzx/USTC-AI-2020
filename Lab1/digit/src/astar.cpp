#include <iostream>
#include <cstring>
#include <cassert>
#include <vector>
#include <unordered_set>
 
using namespace std;

enum direction {UP, RIGHT, DOWN, LEFT};

unordered_set<string> visited_states;

class Node {
    public:
        int state[5][5];
        string state_str = "";
        int pos_i_0s[2] = {-1, -1};
        int pos_j_0s[2] = {-1, -1};
        Node* parent;
        vector<Node*> children;
        vector<int> moves_i;
        vector<int> moves_j;
        vector<direction> moves_d;

        Node(int cur_state[5][5]) {
            for (int i=0; i < 5; i++) {
                for (int j=0; j < 5; j++) {
                    state[i][j] = cur_state[i][j];
                    state_str += to_string(state[i][j]);
                    state_str += ' ';
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
            visited_states.insert("saddfsasfds");
            int n = moves_i.size();
            for (int i=0; i < n; i++) {
                int pos_i = moves_i[i];
                int pos_j = moves_j[i];
                direction direct = moves_d[i];
                Node* new_node = new Node(state);
                new_node->move(pos_i, pos_j, direct);
                new_node->parent = this;
                if (visited_states.find(new_node->state_str) != visited_states.end()){
                    delete new_node;
                } else {
                    children.push_back(new_node);
                    visited_states.insert(new_node->state_str);
                }
            }
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
            update();
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

        void update() {
            state_str = "";
            pos_i_0s[0] = -1;
            pos_i_0s[1] = -1;
            pos_j_0s[0] = -1;
            pos_j_0s[1] = -1;
            for (int i=0; i < 5; i++) {
                for (int j=0; j < 5; j++) {
                    state_str += to_string(state[i][j]);
                    state_str += ' ';
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
};

int main() {
    int init_state[5][5] = {
        {1, 2, 3, 4, 5},
        {7, 7, 8, 9, 10},
        {0, 7, 11, 12, 13},
        {14, 0, 16, 17, 18},
        {19, 20, 21, 6, 15}
    };

    int goal_state[5][5] = {
        {1, 2, 3, 4, 5},
        {7, 7, 8, 9, 10},
        {6, 7, 11, 12, 13},
        {14, 15, 16, 17, 18},
        {19, 20, 21, 0, 0}
    };

    Node* node0 = new Node(init_state);

    node0->show_state();
    visited_states.insert(node0->state_str);
    node0->expend_nodes();
    node0->show_moves();
    node0->show_children_state();

    return 0;
}
