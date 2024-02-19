#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

extern void init();
extern int add(int mId, int mGrade, char mGender[7], int mScore);
extern int remove(int mId);
extern int query(int mGradeCnt, int mGrade[], int mGenderCnt, char mGender[][7], int mScore);

/////////////////////////////////////////////////////////////////////////

#define CMD_INIT 100
#define CMD_ADD 200
#define CMD_REMOVE 300
#define CMD_QUERY 400

static bool run() {
	int q;
	scanf("%d", &q);

	int id, grade, score;
	char gender[7];
	int cmd, ans, ret;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
			case CMD_INIT:
				init();
				okay = true;
				break;
			case CMD_ADD:
				scanf("%d %d %s %d %d", &id, &grade, gender, &score, &ans);
				ret = add(id, grade, gender, score);
				if (ans != ret)
					okay = false;
				break;
			case CMD_REMOVE:
				scanf("%d %d", &id, &ans);
				ret = remove(id);
				if (ans != ret)
					okay = false;
				break;
			case CMD_QUERY: {
				int gradeCnt, genderCnt;
				int gradeArr[3];
				char genderArr[2][7];
				scanf("%d", &gradeCnt);
				if (gradeCnt == 1) {
					scanf("%d %d", &gradeArr[0], &genderCnt);
				} else if (gradeCnt == 2) {
					scanf("%d %d %d", &gradeArr[0], &gradeArr[1], &genderCnt);
				} else {
					scanf("%d %d %d %d", &gradeArr[0], &gradeArr[1], &gradeArr[2], &genderCnt);
				}
				if (genderCnt == 1) {
					scanf("%s %d %d", genderArr[0], &score, &ans);
				} else {
					scanf("%s %s %d %d", genderArr[0], genderArr[1], &score, &ans);
				}
				ret = query(gradeCnt, gradeArr, genderCnt, genderArr, score);
				if (ans != ret)
					okay = false;
				break;
			}
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

struct Student{
    int id, score;
    bool operator<(const Student &other) const{
        if(score!=other.score) return score<other.score;
        return id<other.id;
    }
    bool operator==(const Student &other) const{
        if(score!=other.score) return false;
        return id==other.id;
    }
};

struct Data{
    int grade, gender, score;
};

unordered_map<int, Data> id_map;
set<Student> students[3][2];

void init() {
	id_map.clear();
    for(int i=0;i<3;i++){
        for(int j=0;j<2;j++) students[i][j].clear();
    }
}

int add(int mId, int mGrade, char mGender[7], int mScore) {
    int grade, gender, score;
    grade=mGrade-1, gender=mGender[0]=='m'?0:1, score=mScore;
    id_map[mId]={grade, gender, score};
    Student student={mId, score};
    students[grade][gender].insert(student);
    auto iter=prev(students[grade][gender].end());
    return iter->id;
}

int remove(int mId) {
    if(id_map.count(mId)==0) return 0;
    Data data=id_map[mId];
    students[data.grade][data.gender].erase({mId, data.score});

    if(students[data.grade][data.gender].empty()) return 0;
    else{
        auto iter=students[data.grade][data.gender].begin();
        return iter->id;
    }
}

int query(int mGradeCnt, int mGrade[], int mGenderCnt, char mGender[][7], int mScore) {
	Student ret={-1,300001};
    for(int i=0;i<mGradeCnt;i++){
        for(int j=0;j<mGenderCnt;j++){
            int grade=mGrade[i]-1, gender=mGender[j][0]=='m'?0:1;
            auto iter=students[grade][gender].lower_bound({0,mScore});
            if(iter!=students[grade][gender].end()){
                ret=min(ret, {iter->id, iter->score});
            }
        }
    }
    Student cmp={-1,300001};
    if(ret==cmp) return 0;
    else return ret.id;
}