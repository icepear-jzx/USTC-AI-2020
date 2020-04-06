#include <iostream>
#include <cstring>
#include <cassert>
#include <set>
 
using namespace std;

enum direction {UP, RIGHT, DOWN, LEFT};

class Node {
    public:
        int state[5][5];

        Node(int cur_state[5][5]) {
            memcpy(state, cur_state, 5*5*sizeof(int));
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
};

int main() {
    int init_state[5][5] = {
        {1, 2, 3, 4, 5},
        {7, 7, 8, 9, 10},
        {0, 7, 11, 12, 13},
        {14, 0, 16, 17, 18},
        {19, 20, 21, 0, 0}
    };

    int goal_state[5][5] = {
        {1, 2, 3, 4, 5},
        {7, 7, 8, 9, 10},
        {6, 7, 11, 12, 13},
        {14, 15, 16, 17, 18},
        {19, 20, 21, 0, 0}
    };

    Node Node0(init_state);

    Node0.show_state();

    Node0.move(1, 1, RIGHT);

    Node0.show_state();

    return 0;
}
