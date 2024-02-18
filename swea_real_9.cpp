#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>

struct Result
{
	int mOrder;
	int mRank;
};

extern void init();
extern void search(char mStr[], int count);
extern Result recommend(char mStr[]);
extern int relate(char mStr1[], char mStr2[]);
extern void rank(char mPrefix[], int mRank, char mReturnStr[]);

#define MAX_LENGTH      (7 + 1)
#define CMD_INIT		(100)
#define CMD_SEARCH		(200)
#define CMD_RECOMMEND	(300)
#define CMD_RELATE		(400)
#define CMD_RANK		(500)

static bool run()
{
	int query_num;
	bool okay = false;

	scanf("%d", &query_num);

	for (int q = 0; q < query_num; ++q) {
		int cmd, ret, ans, ans2, mCount, mRank;
		char mStr[MAX_LENGTH], mStr2[MAX_LENGTH], mReturnStr[MAX_LENGTH];
		Result res;

		scanf("%d", &cmd);

		switch (cmd) {
		case CMD_INIT:
			init();
			okay = true;
			break;
		case CMD_SEARCH:
			scanf("%s %d", mStr, &mCount);
			search(mStr, mCount);
			break;
		case CMD_RECOMMEND:
			scanf("%s", mStr);
			res = recommend(mStr);
			printf("CMD_RECOMMEND : %d %d\n", res.mOrder, res.mRank);
			scanf("%d %d", &ans, &ans2);
			if (res.mOrder != ans || res.mRank != ans2)
				okay = false;
			break;
		case CMD_RELATE:
			scanf("%s %s", mStr, mStr2);
			ret = relate(mStr, mStr2);
			scanf("%d", &ans);
			printf("CMD_RELATE : %d\n", ret);
			if (ret != ans)
				okay = false;
			break;
		case CMD_RANK:
			scanf("%s %d", mStr, &mRank);
			rank(mStr, mRank, mReturnStr);
			scanf("%s", mStr2);
			for(int i=0;mReturnStr[i];i++){
				printf("%c", mReturnStr[i]);
			}
			printf("\n");
			if (strncmp(mStr2, mReturnStr, MAX_LENGTH) != 0)
				okay = false;
			break;
		default:
			okay = false;
			break;
		}
	}

	return okay;
}

int main()
{
	setbuf(stdout, NULL);
	//freopen("sample_input9.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}



#include <bits/stdc++.h>
#define ll long long

using namespace std;

const int MAX=15050*7;
int trie[MAX][26], parent[MAX], cnt[MAX];
int node_cnt;
ll hashs[MAX];

int get(int x){
	if(x==parent[x]) return x;
	return parent[x]=get(parent[x]);
}
int unite(int x, int y){
	int p_x=get(x), p_y=get(y);
	if(p_x!=p_y){
		parent[p_x]=p_y;
		cnt[p_y]+=cnt[p_x];
	}
	return cnt[p_y];
}

int get_node(char str[]){
	int node=0;
	for(int i=0;str[i];i++){
		if(trie[node][str[i]-'a']==-1) trie[node][str[i]-'a']=++node_cnt;
		node=trie[node][str[i]-'a'];
	}
	return node;
}

ll get_hash(char str[]){
	ll ret=0;
	for(int i=0;i<7;i++){
		int a=str[i]?(str[i]-'a'+1):0;
		ret<<=5;
		ret+=a;
	}
	return ret;
}

void init()
{
	memset(trie, -1, sizeof(trie));
	memset(cnt, 0, sizeof(cnt));
	memset(hashs, -1, sizeof(hashs));
	for(int i=0;i<MAX;i++) parent[i]=i;
	node_cnt=0;
}

void search(char mStr[], int mCount)
{
	int node=get_node(mStr);
	cnt[get(node)]+=mCount;
	hashs[node]=get_hash(mStr);
}

int relate(char mStr1[], char mStr2[])
{
	return unite(get_node(mStr1), get_node(mStr2));
}

struct datas{
	int node, cnt;
	ll hashs;

	bool operator<(const datas &other) const {
		if(cnt!=other.cnt) return cnt<other.cnt;
		return hashs>other.hashs;
	}
	bool operator>(const datas &other) const {
		if (cnt != other.cnt) return cnt>other.cnt;
		return hashs<other.hashs;
	}
};

struct top5_data{
	datas arr[6];
	int sz=0;
	void push(const datas &d){
		arr[sz++]=d;
		for(int i=sz-1;i>0;i--){
			if(arr[i]>arr[i-1]) swap(arr[i], arr[i-1]);
		}
		if(sz==6) sz=5;
	}
	void clear(){
		sz=0;
	}
};

void dfs(int node, top5_data &top5){
	if(hashs[node]!=-1) top5.push({node, cnt[get(node)], hashs[node]});
	for(int i=0;i<26;i++){
		if(trie[node][i]!=-1){
			dfs(trie[node][i], top5);
		}
	}
}

Result recommend(char mStr[])
{
	Result ret = { 0, 0 };
	ll cmp=get_hash(mStr);
	top5_data top5;
	int node=0;
	dfs(node, top5);
	for(int i=0;i<top5.sz;i++){
		if(top5.arr[i].hashs==cmp){
			cnt[get(get_node(mStr))]++;
			ret = {0,i+1};
			return ret;
		}
	}
	for(int i=0;mStr[i];i++){
		node=trie[node][mStr[i]-'a'];
		top5.clear();
		dfs(node, top5);
		for(int j=0;j<top5.sz;j++){
			if(top5.arr[j].hashs==cmp){
				cnt[get(get_node(mStr))]++;
				ret = {i+1, j+1};
				return ret;
			}
		}
	}
	cnt[get(get_node(mStr))]++;
	return ret;
}

void rank(char mPrefix[], int mRank, char mReturnStr[])
{
	--mRank;
	int node=get_node(mPrefix);
	top5_data top5;
	dfs(node, top5);
	ll ret_hash=top5.arr[mRank].hashs;
	string ret_str="";
	while(ret_hash){
		if(ret_hash%32){
			ret_str+=(ret_hash%32)+'a'-1;
		}
		ret_hash/=32;
	}
	for(int i=ret_str.size()-1, j=0;i>=0;i--,j++){
		mReturnStr[j]=ret_str[i];
	}
	mReturnStr[ret_str.size()]='\0';
	return;
}