// D is the dimension of the U and V matrix
#ifdef D
#else
#define D 5
#endif

// _N is the length of recommender list
#ifdef _N
#else
#define _N 10
#endif

#ifndef FZEOR
#define FZERO 0.1
#endif

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef MF
#define MF 1
#endif

#ifndef BPR
#define BPR 2
#endif

#ifndef IMPLISIT
#define IMPLISIT 1
#endif

#ifndef EXPLISIT
#define EXPLISIT 2
#endif
// _ReviewOnItem is a list node, recording each unit of the rating matrix
typedef struct _ReviewOnItem
{
	int vid;
	float rating;
	short count;
	struct _ReviewOnItem* next;
	/* data */
} REVIEW_ON_ITEM, *PREVIEW_ON_ITEM;

typedef struct _RecommNode
{
	int vid;
	double eval_rating;
	double real_rating;
} RECOMM_NODE, *PRECOMM_NODE;

typedef struct _UserInfo
{
	double avg_rating;
	double afford;
} USER_INFO, *PUSER_INFO;

typedef struct _ItemInfo
{
	double avg_rating;
	double price;
} ITEM_INFO, *PITEM_INFO;

PREVIEW_ON_ITEM* initialRatingMatrix(char* filename, PREVIEW_ON_ITEM* rating_matrix);
void initialFeatureVector(double (*featureV)[D], int num);
double multiVector(double* v1, double* v2, int d);
void initialItemInfo(char* filename, ITEM_INFO* itemlist);
void initialUserInfo(char* filename, USER_INFO* userlist);
void leaveOneOut(PREVIEW_ON_ITEM* test, PREVIEW_ON_ITEM* all, int usernum);
