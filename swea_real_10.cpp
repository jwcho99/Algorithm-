#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define BAR_LEN 5

#define CMD_INIT 100
#define CMD_MAKEWALL 200
#define CMD_MATCHPIECE 300

extern void init();
extern void makeWall(int mHeights[BAR_LEN]);
extern int matchPiece(int mHeights[BAR_LEN]);
extern void output_check();

static bool run()
{
	int N;
	int cmd;
	int heights[BAR_LEN];

	int ret = 0;
	int ans = 0;
	scanf("%d", &N);

	scanf("%d", &cmd);
	bool okay = false;
	if (cmd == CMD_INIT)
	{
		init();
		okay = true;
	}

	for (int turn = 0; turn < N-1; turn++)
	{
		scanf("%d", &cmd);
		for (int i = 0; i < BAR_LEN; i++)
		{
			scanf("%d", &heights[i]);
		}

		switch (cmd)
		{
		case CMD_MAKEWALL:
			makeWall(heights);
			break;
		case CMD_MATCHPIECE:
			ret = matchPiece(heights);
			scanf("%d", &ans);
			printf("CMD_MATCHPIECE : %d\n", ret);
			if (ans != ret)
				okay = false;
			break;
		}
	}
	return okay;
}

int main()
{
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);
	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
		output_check();
	}
	return 0;
}


#include <bits/stdc++.h>
using namespace std;

const int MAX=100001;

vector<int> prv, nxt;
vector<int> hash_map[100001]; //100001

int arr[MAX];
int hash_arr[MAX];
int total_size=0;
int tree[MAX*4];

int seg_query(int node, int left, int right, int start, int end){
	if(end<left || start>right) return 0;
	if(left<=start && end<=right) return tree[node];
	int mid=(start+end)/2;
	return seg_query(node*2,left,right,start,mid)+seg_query(node*2+1,left,right,mid+1,end);
}

void seg_update(int node, int start, int end, int index, int value){
	if(end<index || index<start) return;
	if(start==end){
		tree[node]+=value;
		return;
	}
	int mid=(start+end)/2;
	seg_update(node*2,start,mid,index,value);
	seg_update(node*2+1,mid+1,end,index,value);
	tree[node]=tree[node*2]+tree[node*2+1];
}

void init()
{
	prv.resize(MAX), nxt.resize(MAX);
	iota(prv.begin(), prv.end(), -1);
	iota(nxt.begin(), nxt.end(), 1);
	memset(arr, -1, sizeof(arr));
	memset(hash_arr, -1, sizeof(hash_arr));
	memset(tree,0,sizeof(tree));
	for(int i=0;i<MAX;i++) hash_map[i].clear();
	total_size=0;	
}

int get_min(int index){
	int ret=12;
	int idx=index;
	for(int i=0;i<5;i++){
		ret=min(ret, arr[idx]);
		idx=nxt[idx];
	}
	return ret;
}

int get_hash(int index){
	int ret=0;
	int idx=index;
	int mini=get_min(index);
	for(int i=0;i<5;i++){
		if(idx>=total_size) return -1;
		ret*=10;
		ret+=(arr[idx]-mini);
		idx=nxt[idx];
	}
	return ret;
}

void makeWall(int mHeights[5])
{
	int idx=total_size;
	for(int i=0;i<5;i++) arr[total_size++]=mHeights[i];


	for(int i=0;i<5;i++){
		int h=get_hash(idx);
		if(h!=-1){
			hash_arr[idx]=get_hash(idx);
			hash_map[hash_arr[idx]].push_back(idx);
		}
		idx=prv[idx];
		if(idx==-1) break;
	}
}

int get_piece_hash(int a[5]){
	int ret=0;
	int max_value=-1;
	for(int i=0;i<5;i++) max_value=max(max_value, a[i]);

	for(int i=0;i<5;i++){
		ret*=10;
		ret+=max_value-a[4-i];
	}
	return ret;
}

int matchPiece(int mHeights[5])
{
	int p_hash=get_piece_hash(mHeights);
	if(hash_map[p_hash].empty()) return -1;
	
	auto iter=max_element(hash_map[p_hash].begin(), hash_map[p_hash].end());
	if(iter==hash_map[p_hash].end()) return -43;
	int idx=*iter;
	hash_map[p_hash].erase(iter);
	hash_arr[idx]=-1;
	int last_idx=idx;
	for(int i=0;i<5;i++){
		if(hash_arr[last_idx]!=-1){
			auto iter_tmp=find(hash_map[hash_arr[last_idx]].begin(), hash_map[hash_arr[last_idx]].end(), last_idx);
			hash_map[hash_arr[last_idx]].erase(iter_tmp);
			hash_arr[last_idx]=-1;
		}
		last_idx=nxt[last_idx];
	}
	int ret=idx;

	if(prv[idx]==-1){
		prv[last_idx]=-1;
	}
	else{
		idx=prv[idx];
		nxt[idx]=last_idx;
		prv[last_idx]=idx;

		for(int i=0;i<4;i++){
			int h=hash_arr[idx];
			if(h!=-1){
				auto iter=find(hash_map[h].begin(), hash_map[h].end(), idx);
				if(iter!=hash_map[h].end()){
					hash_map[h].erase(iter);
				}
				else return -4321;
			}
			h=get_hash(idx);
			if(h==-1){
				hash_arr[idx]=-1;
			}
			else{
				hash_arr[idx]=h;
				hash_map[h].push_back(idx);
			}
			idx=prv[idx];
			if(idx==-1) break;
		}
	}
	int ret2=seg_query(1,0,ret-1,0,MAX);
	seg_update(1,0,MAX,ret,5);
	return ret-ret2+1;
}