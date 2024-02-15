#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

struct Result
{
    int top;
    int count;
};

void init(int C);
Result dropBlocks(int mCol, int mHeight, int mLength);

#define CMD_INIT 100
#define CMD_DROP 200

static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    int ans_top, ans_count;
    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);
        if (query == CMD_INIT)
        {
            int C;
            scanf("%d", &C);
            init(C);
            ok = true;
        }
        else if (query == CMD_DROP)
        {
            int mCol, mHeight, mLength;
            scanf("%d %d %d", &mCol, &mHeight, &mLength);
            Result ret = dropBlocks(mCol, mHeight, mLength);
            printf("DROP : %d %d \n", ret.top, ret.count);
            scanf("%d %d", &ans_top, &ans_count);
            if (ans_top != ret.top || ans_count != ret.count)
            {
                ok = false;
            }
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


#include <bits/stdc++.h>

using namespace std;

/*
struct Result{
    int top;
    int count;
};
*/

const int MAX=4010000;
long long total=0;
int _add[MAX];
int _min[MAX], _max[MAX];
int col;

void range_update(int node, int start, int end, int left, int right, int value){
    if(start>right || end<left) return;
    if(start<=left && right<=end){
        _add[node]+=value;
        _min[node]+=value;
        _max[node]+=value;
    }
    else{
        int mid=(left+right)/2;
        range_update(node*2, start, end, left, mid, value);
        range_update(node*2+1, start, end, mid+1, right, value);
        _min[node]=min(_min[node*2], _min[node*2+1])+_add[node];
        _max[node]=max(_max[node*2], _max[node*2+1])+_add[node];
    }
}

void init(int C)
{
    col=C;
    total=0;
    memset(_add,0,sizeof(_add));
    memset(_min,0,sizeof(_min));
    memset(_max,0,sizeof(_max));
}

Result dropBlocks(int mCol, int mHeight, int mLength)
{
    Result ret;
    ret.top = 0;
    ret.count = 0;
    total+=mHeight*mLength;

    range_update(1,mCol,mCol+mLength-1,0,col-1,mHeight);
    int minimum, maximum;
    minimum=_min[1], maximum=_max[1];

    ret.top=maximum-minimum;
    ret.count=(total-1LL*minimum*col)%1000000;

    return ret;
}
