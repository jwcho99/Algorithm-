#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT       100
#define CMD_INSERT     200
#define CMD_MOVECURSOR 300
#define CMD_COUNT      400

extern void init(int H, int W, char mStr[]);
extern void insert(char mChar);
extern char moveCursor(int mRow, int mCol);
extern int countCharacter(char mChar);
extern void checking();

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static bool run()
{
	bool correct = false;
	int queryCnt;
	scanf("%d", &queryCnt);
	int H, W;
	char mChar;
	static char mStr[90001];

	while (queryCnt--)
	{
		int cmd;
		scanf("%d", &cmd);

		if (cmd == CMD_INIT)
		{
			scanf("%d %d %s", &H, &W, mStr);
			init(H, W, mStr);
			correct = true;
		}
		else if (cmd == CMD_INSERT)
		{
			scanf(" %c", &mChar);
			insert(mChar);
		}
		else if (cmd == CMD_MOVECURSOR)
		{
			int mRow, mCol;
			scanf("%d %d", &mRow, &mCol);

			char ret = moveCursor(mRow, mCol);

			char ans;
			scanf(" %c", &ans);
            printf("CMD_MOVECURSOR : %c\n", ret);
			if (ret != ans)
			{
				correct = false;
			}
		}
		else if (cmd == CMD_COUNT)
		{
			scanf(" %c", &mChar);

			int ret = countCharacter(mChar);
            printf("CMD_COUNT : %d\n", ret);
			int ans;
			scanf("%d", &ans);
			if (ret != ans)
			{
				correct = false;
			}
		}
	}
	return correct;
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
        checking();
	}
	return 0;
}

#include <bits/stdc++.h>
using namespace std;

constexpr int MAX=301;
int max_size=0;
int hash_map[MAX][26];
deque<char> dq[MAX];
int h,w;

struct Cursor{
    int x,y;
};

Cursor cursor;

void init(int H, int W, char mStr[])
{
    h=H, w=W;
    cursor.x=0, cursor.y=0;
    max_size=0;
    memset(hash_map, 0, sizeof(hash_map));
    for(int i=0;i<MAX;i++) dq[i].clear();
    
    int k=-1;
    for(int i=0;mStr[i];i++){
        if(i%w==0) k++;
        dq[k].push_back(mStr[i]);
        hash_map[k][mStr[i]-'a']++;
        max_size++;
    }
}

char dq_pop_back(int index){
    char ret=dq[index].back(); dq[index].pop_back();
    hash_map[index][ret-'a']--;
    return ret;
}

void dq_push_front(int index, char c){
    dq[index].push_front(c);
    hash_map[index][c-'a']++;
}


void insert(char mChar)
{
    dq[cursor.x].insert(dq[cursor.x].begin()+cursor.y, mChar);
    hash_map[cursor.x][mChar-'a']++;
    max_size++;
    cursor.y++;
    int nx=cursor.x;
    while(dq[nx].size()>w){
        char tmp=dq_pop_back(nx++);
        dq_push_front(nx,tmp);
    }
    if(cursor.y%w==0){
        cursor.y=0, cursor.x++;
    }
}

char moveCursor(int mRow, int mCol)
{
    int r=mRow-1, c=mCol-1;
    if(r*w+c>=max_size){
        cursor.x=max_size/w;
        cursor.y=max_size%w;
        return '$';
    }
    else{
        cursor.x=r;
        cursor.y=c;
    }
    return dq[cursor.x][cursor.y];
}

int countCharacter(char mChar)
{
    int ret=0;
    int nx=cursor.x, ny=cursor.y;
    printf("cursor %d %d \n", nx, ny);
    for(int i=ny;i<dq[nx].size();i++){
        if(dq[nx][i]==mChar) ret++;
    }
    nx++;
    while(nx<=max_size/w){
        ret+=hash_map[nx][mChar-'a'];
        nx++;
    }
    return ret;
}


void checking(){
    for(int i=0;i<=max_size/w;i++){
        for(int j=0;j<dq[i].size();j++){
            printf("%c", dq[i][j]);
        }
        printf("\n");
    }
}