#include <cstdlib>
#include <iostream>
#include <cstring>
#include <ctime>
#include <queue>

using namespace std;

class Node {
    public:
        int row, col;
        int choice_cnt;
        int degree;

        Node(int in_row, int in_col, int in_choice_cnt, int in_degree) {
            row = in_row;
            col = in_col;
            choice_cnt = in_choice_cnt;
            degree = in_degree;
        }
};

struct CompareNode : public std::binary_function<Node*, Node*, bool>                                                                                     
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        if (lhs->choice_cnt == rhs->choice_cnt) {
            return lhs->degree < rhs->degree;
        } else {
            return lhs->choice_cnt > rhs->choice_cnt;
        }
    }
};

class CSP {
    public:
        enum STATE {GOT, FINISHED, ERROR};
        int state[9][9];
        int degree[9][9];
        int choice[9][9][10];
        int choice_cnt[9][9];
        int cnt;
        priority_queue<Node *, vector<Node *>, CompareNode > unassigned;

        CSP(FILE *fin) {
            input_state(fin);
            update_choice_degree();
            init_unassigned();
            cnt = 0;
        }

        bool backtrack() {
            cnt++;
            int row, col;
            STATE s = select_unassigned(row, col);
            if (s == GOT) {
                for (int value = 1; value < 10; value++) {
                    if (choice[row][col][value] == 1) {
                        if (set_value(row, col, value)) {
                            if (backtrack())
                                return true;
                        }
                        clear_value(row, col, value);
                    }
                }
            } else if (s == FINISHED) {
                return true;
            }
            unassigned.push(new Node(row, col, choice_cnt[row][col], degree[row][col]));
            return false;
        }

        void show_state() {
            for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
                    if (j == 8) printf("%d\n", state[i][j]);
                    else printf("%d ", state[i][j]);
                }
            }
        }

        void show_choice_cnt() {
            for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
                    if (j == 8) printf("%d\n", choice_cnt[i][j]);
                    else printf("%d ", choice_cnt[i][j]);
                }
            }
        }

        void show_degree() {
            for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
                    if (j == 8) printf("%d\n", degree[i][j]);
                    else printf("%d ", degree[i][j]);
                }
            }
        }

        void print_result(FILE *fout) {
            for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
                    if (j == 8) fprintf(fout, "%d\n", state[i][j]);
                    else fprintf(fout, "%d ", state[i][j]);
                }
            }
        }

    private:
        STATE select_unassigned(int &row, int &col) {
            if (unassigned.empty())
                return FINISHED;
            
            while(true) {
                Node *node = unassigned.top();
                unassigned.pop();
                row = node->row;
                col = node->col;
                if (node->choice_cnt == choice_cnt[row][col] && node->degree == degree[row][col]) {
                    if (choice_cnt[row][col] != 0)
                        return GOT;
                    else
                        return ERROR;
                } else {
                    delete node;
                    node = new Node(row, col, choice_cnt[row][col], degree[row][col]);
                    unassigned.push(node);
                }
            }
        }

        bool set_value(int row, int col, int value) {
            bool no_error = true;
            // update state (before updating choice)
            state[row][col] = value;
            // update choice & choice_cnt & degree
            // row:
            for (int j = 0; j < 9; j++) {
                if (state[row][j] == 0) {
                    if (choice[row][j][value] == 1)
                        choice_cnt[row][j] -= 1;
                    choice[row][j][value] -= 1;
                    degree[row][j] -= 1;
                    if (choice_cnt[row][j] == 0) no_error = false;
                }
            }
            // col:
            for (int i = 0; i < 9; i++) {
                if (state[i][col] == 0) {
                    if (choice[i][col][value] == 1)
                        choice_cnt[i][col] -= 1;
                    choice[i][col][value] -= 1;
                    degree[i][col] -= 1;
                    if (choice_cnt[i][col] == 0) no_error = false;
                }
            }
            // box:
            int row_base = row / 3 * 3;
            int col_base = col / 3 * 3;
            for (int i = 0; i < 3; i++){
                for (int j = 0; j < 3; j++){
                    if (state[row_base+i][col_base+j] == 0) {
                        if (choice[row_base+i][col_base+j][value] == 1)
                            choice_cnt[row_base+i][col_base+j] -= 1;
                        choice[row_base+i][col_base+j][value] -= 1;
                        degree[row_base+i][col_base+j] -= 1;
                        if (choice_cnt[row_base+i][col_base+j] == 0) no_error = false;
                    }
                }
            }
            // diag:
            if(row == col){
                for (int i = 0; i < 9; i++){
                    if (state[i][i] == 0) {
                        if (choice[i][i][value] == 1)
                            choice_cnt[i][i] -= 1;
                        choice[i][i][value] -= 1;
                        degree[i][i] -= 1;
                        if (choice_cnt[i][i] == 0) no_error = false;
                    }
                }
            }
            if (row + col == 8) {
                for (int i = 0; i < 9; i++){
                    if (state[i][8-i] == 0) {
                        if (choice[i][8-i][value] == 1)
                            choice_cnt[i][8-i] -= 1;
                        choice[i][8-i][value] -= 1;
                        degree[i][8-i] -= 1;
                        if (choice_cnt[i][8-i] == 0) no_error = false;
                    }
                }
            }
            return no_error;
        }

        void clear_value(int row, int col, int value) {
            // update choice & choice_cnt
            // row:
            for (int j = 0; j < 9; j++) {
                if (state[row][j] == 0) {
                    degree[row][j] += 1;
                    choice[row][j][value] += 1;
                    if (choice[row][j][value] == 1)
                        choice_cnt[row][j] += 1;
                }
            }
            // col:
            for (int i = 0; i < 9; i++) {
                if (state[i][col] == 0) {
                    degree[i][col] += 1;
                    choice[i][col][value] += 1;
                    if (choice[i][col][value] == 1)
                        choice_cnt[i][col] += 1;
                }
            }
            // box:
            int row_base = row / 3 * 3;
            int col_base = col / 3 * 3;
            for (int i = 0; i < 3; i++){
                for (int j = 0; j < 3; j++){
                    if (state[row_base+i][col_base+j] == 0) {
                        degree[row_base+i][col_base+j] += 1;
                        choice[row_base+i][col_base+j][value] += 1;
                        if (choice[row_base+i][col_base+j][value] == 1)
                            choice_cnt[row_base+i][col_base+j] += 1;
                    }
                }
            }
            // diag:
            if(row == col){
                for (int i = 0; i < 9; i++){
                    if (state[i][i] == 0) {
                        degree[i][i] += 1;
                        choice[i][i][value] += 1;
                        if (choice[i][i][value] == 1)
                            choice_cnt[i][i] += 1;
                    }
                }
            }
            if (row + col == 8) {
                for (int i = 0; i < 9; i++){
                    if (state[i][8-i] == 0) {
                        degree[i][8-i] += 1;
                        choice[i][8-i][value] += 1;
                        if (choice[i][8-i][value] == 1)
                            choice_cnt[i][8-i] += 1;
                    }
                }
            }
            // update state (after updating choice)
            state[row][col] = 0;
        }

        void input_state(FILE *fin) {
            for (int i = 0; i < 9; i++)
                for (int j = 0; j < 9; j++)
                    fscanf(fin, "%d", &state[i][j]);
        }

        void update_choice_degree() {
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    choice_cnt[i][j] = 9;
                    for (int value = 0; value < 10; value++) {
                        choice[i][j][value] = 1;
                    }
                    degree[i][j] = 20;
                    if (i == j)
                        degree[i][j] += 6;
                    if (i + j == 8)
                        degree[i][j] += 6;
                }
            }
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (state[i][j] != 0) {
                        set_value(i, j, state[i][j]);
                    }
                }
            }
        }

        void init_unassigned() {
            for (int i = 0; i < 9; i++) {
                for (int j = 0; j < 9; j++) {
                    if (state[i][j] == 0) {
                        Node *node = new Node(i, j, choice_cnt[i][j], degree[i][j]);
                        unassigned.push(node);
                    }
                }
            }
        }
};

int main() {
    char input_dir[30] = "../input/";
    char output_dir[30] = "../output/";
    char filename[20];
    printf("Input filename：\n");
    scanf("%s", filename);
    strcat(input_dir, filename);
    strcat(output_dir, filename);

    FILE *fin = fopen(input_dir, "r");
    CSP *csp = new CSP(fin);
    fclose(fin);

    clock_t start = clock();
    csp->backtrack();
    cout << "Total steps: " << csp->cnt << endl;
    cout << "Total time: " << (clock() - start) / (float)CLOCKS_PER_SEC << endl;

    FILE *fout = fout = fopen(output_dir, "w");
    csp->print_result(fout);
    fclose(fout);

    return 0;
}
