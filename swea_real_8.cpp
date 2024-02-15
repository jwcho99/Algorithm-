#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init(int N, int mPopulation[]);
extern int expand(int M);
extern int calculate(int mFrom, int mTo);
extern int divide(int mFrom, int mTo, int K);

/////////////////////////////////////////////////////////////////////////

#define MAX_N				10000

#define CMD_INIT			100
#define CMD_EXPAND			200
#define CMD_CALCULATE		300
#define CMD_DIVIDE			400

static bool run()
{
	int population[MAX_N];
	int cmd, ans, ret;
	int Q = 0;
	int N, from, to, num;
	bool okay = false;

	scanf("%d", &Q);

	for (int q = 0; q < Q; ++q)
	{
		scanf("%d", &cmd);

		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d", &N);

			for (int i = 0; i < N; i++)
				scanf("%d", &population[i]);

			init(N, population);
			okay = true;
			break;

		case CMD_EXPAND:
			scanf("%d", &num);
			ret = expand(num);
			scanf("%d", &ans);
            printf("EXPAND : %d\n", ret);
			if (ret != ans)
				okay = false;
			break;

		case CMD_CALCULATE:
			scanf("%d %d", &from, &to);
			ret = calculate(from, to);
			scanf("%d", &ans);
            printf("CALCULATE : %d\n", ret);
			if (ret != ans)
				okay = false;
			break;

		case CMD_DIVIDE:
			scanf("%d %d %d", &from, &to, &num);
			ret = divide(from, to, num);
			scanf("%d", &ans);
            printf("DIVIDE : %d\n", ret);
			if (ret != ans)
				okay = false;
			break;

		default:
			okay = false;
		}
	}

	return okay;
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

#include<bits/stdc++.h>
using namespace std;
const int MAX = 10001;
struct Road{
    int time, lane, index;
};

struct cmp{
    bool operator()(const Road &a, const Road &b){
        if(a.time != b.time) return a.time<b.time;
        return a.index>b.index;
    }
};
priority_queue<Road, vector<Road>, cmp> pq;
int city[MAX];
Road roads[MAX];
int tree[4*MAX];
int num;

void seg_init(int node, int start, int end){
    if(start==end){
        tree[node]=roads[start].time;
        return;
    }
    int mid=(start+end)/2;
    seg_init(node*2, start, mid);
    seg_init(node*2+1, mid+1, end);
    tree[node]=tree[node*2]+tree[node*2+1];
}

int seg_query(int node, int left, int right, int start, int end){
    if(start>right || end<left) return 0;
    if(left<=start && end <=right) return tree[node];
    int mid=(start+end)/2;
    return seg_query(node*2, left, right, start, mid)+seg_query(node*2+1, left, right, mid+1, end);
}

void seg_update(int node, int start, int end, int index, int value){
    if(index<start || index>end) return;
    if(start==end){
        tree[node]=value;
        return;
    }
    int mid=(start+end)/2;
    seg_update(node*2, start, mid, index, value);
    seg_update(node*2+1, mid+1, end, index, value);
    tree[node]=tree[node*2]+tree[node*2+1];
}

void init(int N, int mPopulation[])
{
    pq=priority_queue<Road, vector<Road>, cmp>();
    num=N;
    for(int i=0;i<num;i++){
        city[i]=mPopulation[i];
        if(i>0){
            roads[i-1]={city[i]+city[i-1], 1, i-1};
            pq.push(roads[i-1]);
        }
    }
    seg_init(1,0,num-2);
}

int expand(int M)
{
    int ret=0;
    while(M--){
        Road r=pq.top(); pq.pop();
        r.lane++;
        r.time=(city[r.index]+city[r.index+1])/r.lane;
        pq.push(r);
        seg_update(1,0,num-2,r.index, r.time);
        if(M==0) ret=r.time;
    }
    return ret;
}

int calculate(int mFrom, int mTo)
{
    if(mFrom>mTo) swap(mFrom, mTo);
    return seg_query(1,mFrom,mTo-1,0,num-2);
}

int divide(int mFrom, int mTo, int K)
{
    int left=1, right=10000001;
    int mid;

    while(left<=right){
        int count=0;
        mid=(left+right)/2;
        for(int i=mFrom;i<=mTo&&count<=K;){
            int sum=0;
            int j=i;
            while(j<=mTo && sum+city[j]<=mid){
                sum+=city[j++];
            }
            count++;
            i=j;
        }
        if(count<=K){
            right=mid-1;
        }
        else left=mid+1;
    }
    return right+1;
}