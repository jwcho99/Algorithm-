#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT			(100)
#define CMD_CONNECT			(200)
#define CMD_MEASURE			(300)
#define CMD_TEST			(400)

extern void init(int mDevice);
extern void connect(int mOldDevice, int mNewDevice, int mLatency);
extern int measure(int mDevice1, int mDevice2);
extern int test(int mDevice);

static bool run()
{
	int Q;
	int mDevice, mOldDevice, mNewDevice, mDevice1, mDevice2;
	int mLatency;
	
	int ret = -1, ans;

	scanf("%d", &Q);

	bool okay = false;

	for (int q = 0; q < Q; ++q)
	{
		int cmd;
		scanf("%d", &cmd);
		switch(cmd)
		{
		case CMD_INIT:
			scanf("%d", &mDevice);
			init(mDevice);
			okay = true;
			break;
		case CMD_CONNECT:
			scanf("%d %d %d", &mOldDevice, &mNewDevice, &mLatency);
			connect(mOldDevice, mNewDevice, mLatency);
			break;
		case CMD_MEASURE:
			scanf("%d %d", &mDevice1, &mDevice2);
			ret = measure(mDevice1, mDevice2);
            printf("MEASURE : %d\n", ret);
			scanf("%d", &ans);
			if (ret != ans)
				okay = false;
			break;
		case CMD_TEST:
			scanf("%d", &mDevice);
			ret = test(mDevice);
            printf("TEST : %d\n", ret);
			scanf("%d", &ans);
			if (ret != ans)
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
	//freopen("sample_input.txt", "r", stdin);

	int TC, MARK;

	scanf("%d %d", &TC, &MARK);
	for (int tc = 1; tc <= TC; ++tc)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}

#include <bits/stdc++.h>

using namespace std;
const int MAX=10100;

unordered_map<int,int> index_map;
int parent[MAX], dist_to_parent[MAX], depth[MAX];
int firstid[MAX], secondid[MAX];
int first_dist[MAX], second_dist[MAX];
int sz=1;


int to_index(int x){
    if(index_map.count(x)==0) index_map[x]=sz++;
    return index_map[x];
}

void init(int mDevice)
{
    index_map.clear();
    for(int i=0;i<sz;i++){
        firstid[i]=secondid[i]=0;
        first_dist[i]=second_dist[i]=0;
    }
    sz=1;
    int device=to_index(mDevice);
    parent[device]=dist_to_parent[device]=depth[device]=0;   
}

void update(int parent_id, int child){
    int tmp=0;
    while(parent_id){
        tmp+=dist_to_parent[child];
        if(firstid[parent_id]==child){
            if(first_dist[parent_id]<tmp) first_dist[parent_id]=tmp;
        }
        else if(first_dist[parent_id]<tmp){
            second_dist[parent_id]=first_dist[parent_id];
            secondid[parent_id]=firstid[parent_id];
            first_dist[parent_id]=tmp;
            firstid[parent_id]=child;
        }
        else if(second_dist[parent_id]<tmp){
            second_dist[parent_id]=tmp;
            secondid[parent_id]=child;
        }
        else break;
        child=parent_id;
        parent_id=parent[parent_id];
    }
}

void connect(int mOldDevice, int mNewDevice, int mLatency)
{
    int device1, device2;
    device1=to_index(mOldDevice);
    device2=to_index(mNewDevice);

    parent[device2]=device1;
    dist_to_parent[device2]=mLatency;
    depth[device2]=depth[device1]+1;

    update(device1, device2);
}

int measure(int mDevice1, int mDevice2)
{
    int device1, device2;
    device1=to_index(mDevice1);
    device2=to_index(mDevice2);

    if(depth[device2]<depth[device1]) swap(device1, device2);
    int dif=depth[device2]-depth[device1];
    int ret=0;
    while(dif--){
        ret+=dist_to_parent[device2];
        device2=parent[device2];
    }
    while(device1!=device2){
        ret+=dist_to_parent[device1]+dist_to_parent[device2];
        device1=parent[device1]; device2=parent[device2];
    }
    return ret;
}


int test(int mDevice)
{
    int device=to_index(mDevice);
    int ret=first_dist[device]+second_dist[device];

    int p=parent[device];
    int tmp=first_dist[device];
    while(p){
        tmp+=dist_to_parent[device];
        if(second_dist[p]){
            if(firstid[p]==device){
                ret=max(ret, tmp+second_dist[p]);
            }
            else{
                ret=max(ret, tmp+first_dist[p]);
            }
        }
        else{
            ret=max(ret, tmp);
        }
        device=p; p=parent[p];
    }
    return ret;
}
