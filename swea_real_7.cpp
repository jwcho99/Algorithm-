#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init(int N, int K);
extern void registerUser(int mTime, int mUID, int mNum, int mGroupIDs[]);
extern int offerNews(int mTime, int mNewsID, int mDelay, int mGroupID);
extern void cancelNews(int mTime, int mNewsID);
extern int checkUser(int mTime, int mUID, int mRetIDs[]);

/////////////////////////////////////////////////////////////////////////

#define INIT	0
#define REGI	1
#define OFFER	2
#define CANCEL	3
#define CHECK	4

static int gids[30];
static int ansids[3];
static int retids[3];
static bool run()
{
	int N, K;
	int cmd, ans, ret;	
	int time, num, uid, gid, nid, delay;

	int Q = 0;
	bool okay = false;

	scanf("%d", &Q);
	for (int q = 0; q < Q; ++q)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case INIT:
			scanf("%d %d", &N, &K);
			init(N, K);
			okay = true;
			break;

		case REGI:
			scanf("%d %d %d", &time, &uid, &num);
			for (int m = 0; m < num; m++) {
				scanf("%d", &gids[m]);
			}
			registerUser(time, uid, num, gids);
			break;

		case OFFER:
			scanf("%d %d %d %d %d", &time, &nid, &delay, &gid, &ans);
			ret = offerNews(time, nid, delay, gid);
			if (ans != ret) {
				okay = false;
			}
			break;

		case CANCEL:
			scanf("%d %d", &time, &nid);
			cancelNews(time, nid);
			break;

		case CHECK:
			scanf("%d %d %d", &time, &uid, &ans);
			ret = checkUser(time, uid, retids);

			num = ans;
			if (num > 3) num = 3;
			for (int m = 0; m < num; m++) {
				scanf("%d", &ansids[m]);
			}
			if (ans != ret)	{
				okay = false;
			}
			else {
				for (int m = 0; m < num; m++) {
					if (ansids[m] != retids[m]) {
						okay = false;
					}
				}
			}
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
	//freopen("sample_input.txt", "r", stdin);

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

int n, k;
unordered_map<int, int> user_id;
unordered_map<int, int> channel_id;

struct News {
	int id, time;
};

struct cmp {
	/*
	bool operator()(News& a, News& b) {
		if (a.time != b.time) return a.time > b.time;
		return a.id > b.id;
	}
	*/
	bool operator() (pair<News,int>&a , pair<News,int>&b){
		if (a.first.time != b.first.time) return a.first.time > b.first.time;
		return a.first.id > b.first.id;
	}
};

struct rev_cmp {
	bool operator()(News& a, News& b) {
		if (a.time != b.time) return a.time < b.time;
		return a.id < b.id;
	}
};

//priority_queue<News, vector<News>, rev_cmp> users[500];
vector<News> users[500];
//priority_queue<News, vector<News>, cmp> channels[500];
priority_queue<pair<News, int>, vector<pair<News,int>>, cmp> all_news;


vector<int> follower[500];
vector<int> following[500];
unordered_map<int, bool> canceled;

int to_uid(int x) {
	return user_id[x];
}

int to_channelid(int x) {
	return channel_id[x];
}
void init(int N, int K)
{
	n = N, k = K;
	user_id.clear(); channel_id.clear();
	canceled.clear();
	all_news = priority_queue<pair<News, int>, vector<pair<News, int>>, cmp >();
	for (int i = 0; i < 500; i++) {
		//users[i] = priority_queue<News, vector<News>, rev_cmp>();
		users[i].clear();
		//channels[i] = priority_queue<News, vector<News>, cmp>();
		follower[i].clear();
		following[i].clear();
	}
}

void sendNews(int mTime) {
	while (!all_news.empty()) {
		auto &[news, cid] = all_news.top();
		if (news.time > mTime) break;
		if (canceled[news.id]) {
			all_news.pop();
			continue;
		}
		for (auto x : follower[cid]) users[x].push_back(news);
		all_news.pop();
	}
	/*
	while (!channels[i].empty()) {
		if (channels[i].top().time > mTime) break;

		if (canceled[channels[i].top().id]) {
			channels[i].pop();
			continue;
		}
		for (auto x : follower[i]) {
			users[x].push_back(channels[i].top());
		}
		channels[i].pop();
	}
	*/
}

void registerUser(int mTime, int mUID, int mNum, int mChannelIDs[])
{
	if (user_id.count(mUID) == 0) user_id[mUID] = user_id.size();
	int uid = to_uid(mUID);
	sendNews(mTime);
	for (int i = 0; i < mNum; i++) {
		if (channel_id.count(mChannelIDs[i]) == 0) channel_id[mChannelIDs[i]] = channel_id.size();
		int cid = to_channelid(mChannelIDs[i]);
		follower[cid].push_back(uid);
		following[uid].push_back(cid);
	}
}

int offerNews(int mTime, int mNewsID, int mDelay, int mChannelID)
{
	News news = { mNewsID, mTime + mDelay };
	int cid = to_channelid(mChannelID);
	//channels[cid].push(news);
	all_news.push({ news, cid });
	canceled[mNewsID] = false;

	return follower[cid].size();
}

void cancelNews(int mTime, int mNewsID)
{
	canceled[mNewsID] = true;
}

int checkUser(int mTime, int mUID, int mRetIDs[])
{
	int uid = to_uid(mUID);
	sendNews(mTime);
	int count = 0;

	//sort(users[uid].begin(), users[uid].end(), rev_cmp());

	while (!users[uid].empty()) {
		News news = users[uid].back(); users[uid].pop_back();
		if (canceled[news.id]) continue;

		mRetIDs[count++] = news.id;
		if (count == 3) break;
	}
	while (!users[uid].empty()) {
		News news = users[uid].back(); users[uid].pop_back();
		if (canceled[news.id]) continue;
		count++;
	}
	return count;
}