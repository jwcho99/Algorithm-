#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init(int N, int mId[], int mNum[]);
extern int add(int mId, int mNum, int mParent);
extern int remove(int mId);
extern int distribute(int K);

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_REMOVE 3
#define CMD_DISTRIBUTE 4

static bool run() {
	int q;
	scanf("%d", &q);

	static int midArr[1000], mnumArr[1000];
	int mid, mnum, mparent, n, k;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
			case CMD_INIT:
				scanf("%d", &n);
				for (int j = 0; j < n; ++j) {
					scanf("%d %d", &midArr[j], &mnumArr[j]);
				}
				init(n, midArr, mnumArr);
				okay = true;
				break;
			case CMD_ADD:
				scanf("%d %d %d %d", &mid, &mnum, &mparent, &ans);
				ret = add(mid, mnum, mparent);
                printf("ADD : %d\n", ret);
				if (ans != ret)
					okay = false;
				break;
			case CMD_REMOVE:
				scanf("%d %d", &mid, &ans);
				ret = remove(mid);
                printf("REMOVE : %d\n", ret);
				if (ans != ret)
					okay = false;
				break;
			case CMD_DISTRIBUTE:
				scanf("%d %d", &k, &ans);
				ret = distribute(k);
                printf("DISTRIBUTE : %d\n", ret);
				if (ans != ret)
					okay = false;
				break;
			default:
				okay = false;
				break;
		}
	}
	return okay;
}

int main() {
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}

#include <bits/stdc++.h>

using namespace std;

unordered_map<int,int> to_id;
int tree_num=0;
int group;

struct node{
    int total, parent, child_cnt;
    int child[3];
    bool removed;
};

node tree[20000];

void init(int N, int mId[], int mNum[]) {
    group=N;
    tree_num=0;
    to_id.clear();
    for(int i=0;i<group;i++){
        to_id[mId[i]]=tree_num++;
        int idx=to_id[mId[i]];
        tree[idx]={mNum[i], -1, 0, {-1,-1,-1}, false};
    }
}

int add(int mId, int mNum, int mParent) {
    to_id[mId]=tree_num++;
    if(tree[to_id[mParent]].child_cnt==3) return -1;
    int idx=to_id[mId];
    tree[idx]={mNum, -1, 0, {-1,-1,-1}, false};
    int parent=to_id[mParent];
    tree[parent].child[tree[parent].child_cnt++]=idx;
    tree[idx].parent=parent;

    while(parent!=-1){
        tree[parent].total+=mNum;
        parent=tree[parent].parent;
    }
    return tree[to_id[mParent]].total;
}

int remove(int mId) {
    if(to_id.count(mId)==0) return -1;
    int idx=to_id[mId];
    while(idx!=-1){
        if(tree[idx].removed) return -1;
        idx=tree[idx].parent;
    }
    idx=to_id[mId];
    int ret=tree[idx].total;
    int parent=tree[idx].parent;
    tree[parent].child_cnt--;
    for(int i=0;i<3;i++){
        if(tree[parent].child[i]==idx){
            tree[parent].child[i]=-1;
            break;
        }
    }    
    while(parent!=-1){
        tree[parent].total-=ret;
        parent=tree[parent].parent;
    }
    tree[idx].removed=true;
    return ret;
}

int distribute(int K) {
	vector<int> distri;
    for(int i=0;i<group;i++) distri.push_back(tree[i].total);

    sort(distri.begin(), distri.end());
    int s=accumulate(distri.begin(), distri.end(), 0);
    if(s<=K) return distri[group-1];
    for(int i=group-1;i>=0;i--){
        s-=distri[i];
        int chk=(K-s)/(group-i);
        int compare=0;
        if(i>=1) compare=distri[i-1];
        if(compare<=chk) return chk; 
    }
    return 0;
}