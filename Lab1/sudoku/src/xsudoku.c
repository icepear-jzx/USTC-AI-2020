#include <stdio.h>
#include <string.h>


int map[9][9];
int assign[9][9];
int step;
int flag;


void GetInput(FILE *fin) {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            fscanf(fin, "%d", &map[i][j]);
    return;
}


void InitSudoku(){
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++){
            if (map[i][j] != 0){
                assign[i][j] = 1;
            }
            else assign[i][j] = 0;
        }
    }
    return;
}


void PrintSudoku() {
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (j == 8) printf("%d\n", map[i][j]);
            else printf("%d ", map[i][j]);
        }
    }
    printf("step: %d\n", step);
    return;
}


void FPrintSudoku(FILE * fout) {
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (j == 8) fprintf(fout, "%d\n", map[i][j]);
            else fprintf(fout, "%d ", map[i][j]);
        }
    }
    return;
}


int IsConsistent(int row, int col, int value) {
    for (int i = 0; i < 9; i++) {
        if (i!= col && map[row][i] == value) return 0;
        if (i != row && map[i][col] == value) return 0;
    }
    //小方块
    int box_row = row / 3 * 3;
    int box_col = col / 3 * 3;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if(box_row + i != row && box_col + j != col)
                if (map[box_row + i][box_col + j] == value)
                    return 0;
        }
    } 
    // X
    if(row == col){
        for (int i = 0; i < 9; i++){
            if (i != row && map[i][i] == value) return 0;
        }
    }
    if (row == 8-col) {
        for (int i = 0; i < 9; i++){
            if (i != row && map[i][8-i] == value) return 0;
        }
    }
    return 1;
}


int DegreeHeuristic(int row, int col) {
    // 返回(row, col)元素的度（有多少约束）
    int degree = 0;
    for (int i = 0;i < 9;i++) {
        // 行
        if (i != col && assign[row][i] == 1) degree++;
        // 列
        if (i != row && assign[i][col] == 1) degree++;
    }
    // 小方块
    int box_row = row / 3 * 3;
    int box_col = col / 3 * 3;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if(box_row + i != row && box_col + j != col)
                if (assign[box_row + i][box_col + j] == 1)
                    degree++;
        }
    }
    if(row == col){
        for (int i = 0; i < 9; i++){
            if (i != row && assign[i][i] == 1) degree++;
        }
    }
    if (row == 8-col) {
        for (int i = 0; i < 9; i++){
            if (i != row && assign[i][8-i] == 1) degree++;
        }
    }
    return degree;
}


int MRVHeuristic(int row, int col) {
    // 返回(row, col)元素的可取值的个数
    int mrv = 0;
    for (int i = 1; i <= 9; i++){
        if (IsConsistent(row, col, i))
            mrv++;
    }
    return mrv;
}


int SelectUnassigned() {
    // 找到未解决的位置中度最大的一个，若返回-1则全部解决，-2则无解
    int row = 0;
    int col = 0;
    // int degree = 0;
    int mrv = 10;
    // for (int i = 0; i < 9; i++){
    //     for (int j = 0; j < 9; j++){
    //         if (assign[i][j] == 0) {
    //             if (degree < DegreeHeuristic(i, j)) {
    //                 row = i;
    //                 col = j;
    //                 degree = DegreeHeuristic(i, j);
    //             }
    //         }
    //     }
    // }

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (assign[i][j] == 0) {
                if (MRVHeuristic(i, j)){
                    if (mrv > MRVHeuristic(i, j)) {
                        row = i;
                        col = j;
                        mrv = MRVHeuristic(i, j);
                    }
                }
                else {
                    // 如果mrv是0，表示出现无可取值的元素，无解，返回-2
                    return -2;
                }
            }
        }
    }

    // if (degree) return row * 9 + col;
    // 如果mrv是10，表示没有未解决的元素，返回-1
    if (mrv != 10) return row * 9 + col;
    return -1;
}


int GetNextPossibleAns (int row, int col){
    for(int i = map[row][col]+1; i < 10; i++)
        if(IsConsistent(row, col, i))
            return i;
    return 0;
}


void BackTracking() {
    int next = SelectUnassigned();
    if (next == -1) {
        PrintSudoku();
        flag = 1;
        return;
    }
    if (next == -2) {
        return;
    }
    int row = next / 9;
    int col = next % 9;
    while ((map[row][col] = GetNextPossibleAns(row, col)) != 0) {
        assign[row][col] = 1;
        step++;
        BackTracking();
        if(flag) break;
        assign[row][col] = 0;
    }
    return;
}


int main() {
    char input_dir[30] = "../input/";
    char output_dir[30] = "../output/";
    char filename[20];
    printf("Input filename：\n");
    scanf("%s",filename);
    strcat(input_dir,filename);
    strcat(output_dir,filename);

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

    GetInput(fin);
    InitSudoku();
    step = 0;
    flag = 0;
    BackTracking();
    if (flag) FPrintSudoku(fout);
    return 0;
}