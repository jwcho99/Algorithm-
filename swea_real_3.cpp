#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAX_ROW 40
#define MAX_COL 30

struct Result{
    int row;
    int col;
};

void init(int mRows, int mCols, int mCells[MAX_ROW][MAX_COL]);
Result putPuzzle(int mPuzzle[3][3]);
void clearPuzzles();


#define CMD_INIT 1
#define CMD_PUT 2
#define CMD_CLR 3


static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);
        if (query == CMD_INIT)
        {
            int mRows, mCols;
            int mCells[MAX_ROW][MAX_COL];
            scanf("%d %d", &mRows, &mCols);
            for (int i = 0; i < mRows; i++){
                for (int j = 0; j < mCols; j++){
                    scanf("%d", &mCells[i][j]);
                }
            }
            init(mRows, mCols, mCells);
            ok = true;
        }
        else if (query == CMD_PUT)
        {
            char strPuzzle[10];
            int mPuzzle[3][3];
            int ans_row, ans_col;
            scanf("%s", strPuzzle);
            int cnt = 0;
            for (int i = 0; i < 3; i++){
                for (int j = 0; j < 3; j++){
                    mPuzzle[i][j] = strPuzzle[cnt] - '0';
                    cnt++;
                }
            }
            Result ret = putPuzzle(mPuzzle);
            scanf("%d %d", &ans_row, &ans_col);
            if (ans_row != ret.row || ans_col != ret.col)
            {
                ok = false;
            }
        }
        else if (query == CMD_CLR)
        {
            clearPuzzles();
        }
    }
    return ok;
}


int main()
{
    setbuf(stdout, NULL);
    // freopen("sample_input.txt", "r", stdin);
    int T, MARK;
    scanf("%d %d", &T, &MARK);
    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }
    return 0;
}

#define MAX_ROW 40
#define MAX_COL 30

#include <bits/stdc++.h>

using namespace std;

const vector<vector<pair<int, int>>> pattern = {
    {{0, 0}, {1, 0}, {1, 1}, {1, 2}, {2, 2}},
    {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
    {{0, 0}, {0, 1}, {0, 2}},
    {{0, 0}, {1, 0}, {2, 0}},
    {{0, 0}, {0, 1}}
};

vector<pair<int, int>> hash_map[5][1 << 16];
vector<pair<int, int>> used;
bitset<30> visited[40];
int n, m;


void init(int mRows, int mCols, int board[MAX_ROW][MAX_COL]){
    n = mRows, m = mCols;
    for (int i = 0; i < n; i++) visited[i].reset();
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < (1 << 15); j++) hash_map[i][j].clear();
    }
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int type = 0; type < 5; type++) {
                int min_num = INT_MAX;
                bool flag = true;
                for (auto [dx, dy] : pattern[type]) {
                    if (i + dx >= n || j + dy >= m) {
                        flag = false;
                        break;
                    }
                    min_num = min(min_num, board[i + dx][j + dy]);
                }
                if (!flag) continue;
                int hash = 0;
                for (auto [dx, dy] : pattern[type]) {
                    hash <<= 3;
                    hash += board[i + dx][j + dy] - min_num;
                }
                hash_map[type][hash].push_back({ i,j });
                used.push_back({ type,hash });
            }
        }
    }
    sort(used.begin(), used.end());
    used.resize(unique(used.begin(), used.end()) - used.begin());

    for (auto [i, j] : used) sort(hash_map[i][j].begin(), hash_map[i][j].end());
}

Result putPuzzle(int mPuzzle[3][3])
{
    int type = -1;
    for (int t = 0; t < 5; t++) {
        bool flag = true;
        for (auto [i, j] : pattern[t]) {
            if (mPuzzle[i][j] == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            type = t;
            break;
        }
    }
    int min_num = INT_MAX;
    for (auto [i, j] : pattern[type]) min_num = min(min_num, mPuzzle[i][j]);

    int hash = 0;
    for (auto [i, j] : pattern[type]) {
        hash <<= 3;
        hash += mPuzzle[i][j] - min_num;
    }
    Result ret = { -1, -1 };

    bool flag = false;
    for (auto [i, j] : hash_map[type][hash]) {
        flag = true;
        for (auto [dx, dy] : pattern[type]) {
            if (visited[i + dx].test(j + dy)) {
                flag = false;
                break;
            }
        }
        if (!flag) continue;
        ret = { i,j };
        for (auto [dx, dy] : pattern[type]) {
            visited[i + dx].set(j + dy);
        }
        break;
    }
    return ret;
}

void clearPuzzles()
{
    for (int i = 0; i < n; i++) visited[i].reset();
    return;
}
