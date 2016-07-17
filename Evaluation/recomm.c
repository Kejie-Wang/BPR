#include <stdio.h>
#include <math.h>
#include <time.h>
#include "../GeneralStruct/gvStruct.h"
#include "recomm.h"

double rmse(double(*U)[D],double(*V)[D],double*bu,double*bv,double*item_mean,PREVIEW_ON_ITEM* \
	rm_test,PREVIEW_ON_ITEM* rm_train, int usernum, int itemnum,double total_mean)
{
	double mse=0;
	int mse_count=0;
	int debug_count=0;

	for (int i = 1; i < usernum; ++i)
	{
		PREVIEW_ON_ITEM point_rating;
		point_rating = rm_test[i];
		while (point_rating)
		{
			double eval = 0;
			if (rm_train[i] && item_mean[point_rating->vid]!=-1)
			{
				for (int d = 0; d < D; ++d)
				{
					eval+=U[i][d]*V[point_rating->vid][d];
				}
				eval+=bu[i]+bv[point_rating->vid];
			}
			else if (!rm_train[i] && item_mean[point_rating->vid]!=-1)
			{
				eval = item_mean[point_rating->vid];
			}
			else if (item_mean[point_rating->vid] == -1)
			{
				eval = total_mean;
				debug_count++;
			}

			mse += (eval - point_rating->rating) * (eval - point_rating->rating);
			mse_count++;
			point_rating = point_rating->next;
		}
	}
	return sqrt(mse/mse_count);
}

void makeRecommend(double(*U)[D],double(*V)[D],double*bu,double*bv,PREVIEW_ON_ITEM*rm_train,\
	double*item_mean, RECOMM_NODE(*recomm_list)[_N], int usernum, int itemnum,double total_mean)
{
	// initialize the recommend list
	for (int i = 1; i < usernum; ++i)
	{
		for (int j = 0; j < _N; ++j)
		{
			recomm_list[i][j].eval_rating = 0;
			recomm_list[i][j].real_rating = 0;
			recomm_list[i][j].vid = -1;
		}
	}
	// for each user
	for (int uid = 1; uid < usernum; ++uid)
	{
		if (DEBUG)
		{
			printf("%d\n", uid);
		}
		// used to filter out the items in training set
		int *filter_item = (int *)malloc(sizeof(int) * itemnum);
		for (int i = 0; i < itemnum; ++i)
		{
			filter_item[i] = 0;
		}
		PREVIEW_ON_ITEM point_rating;
		point_rating = rm_train[uid];
		while (point_rating)
		{
			filter_item[point_rating->vid] = 1;
			point_rating = point_rating->next;
		}

		// for each item
		for (int vid = 1; vid < itemnum; ++vid)
		{
			// filter out the items in+ traing set
			if(filter_item[vid])
			{
				continue;
			}
			// PREVIEW_ON_ITEM point_rating;
			// point_rating = rm_train[uid];
			// int flag_pass = 0;
			// while (point_rating)
			// {
			// 	if (point_rating->vid == vid)
			// 	{
			// 		flag_pass = 1;
			// 		break;
			// 	}
			// 	point_rating = point_rating->next;
			// }
			// if (flag_pass)
			// {
			// 	continue;
			// }
			// the evaluation uid's rating on vid
			double eval = 0;
			// first case, uid and vid all in training set
			if (rm_train[uid] && item_mean[vid]!=-1)
			{
				eval = multiVector(U[uid],V[vid],D);
				eval+=bu[uid]+bv[vid];
			}
			// second case, uid is not in traing set, vid is in training set
			// use item_mean as the evaluation
			else if (!rm_train[uid] && item_mean[vid]!=-1)
			{
				eval = item_mean[vid];
			}
			// third case, uid and vid are both not in training set
			// user total_mean as the evaluation
			else if (item_mean[vid] == -1)
			{
				eval = total_mean;
			}

			// rank the item by evaluation
			for (int n = 0; n < _N; ++n)
			{
				if (eval>recomm_list[uid][n].eval_rating)
				{
					if (n==0)
					{
						recomm_list[uid][n].eval_rating = eval;
						recomm_list[uid][n].vid = vid;
					}
					else
					{
						recomm_list[uid][n-1].eval_rating = recomm_list[uid][n].eval_rating;
						recomm_list[uid][n-1].vid = recomm_list[uid][n].vid;
						recomm_list[uid][n].eval_rating = eval;
						recomm_list[uid][n].vid = vid;
					}
				}
			}
		}

		free(filter_item);
	}
}
double Precision(RECOMM_NODE(*recomm_list)[_N],PREVIEW_ON_ITEM* rm_test, int usernum)
{
	double precision = 0;
	int totalhit = 0;
	int testnum = 0;
	for (int i = 1; i < usernum; ++i)
	{
		int hit=0;
		PREVIEW_ON_ITEM point_rating;
		point_rating = rm_test[i];
		if (point_rating==NULL)
		{
			continue;
		}
		testnum++;
		while (point_rating)
		{
			if (point_rating->rating > 0)
			{
				for (int n = 0; n < _N; ++n)
				{
					if (recomm_list[i][n].vid == point_rating->vid)
					{
						hit++;
						totalhit++;
						// printf("%d %d\n", i, totalhit);
						break;
					}
				}
			}
			point_rating = point_rating->next;
		}
		precision += (double)hit/_N;
	}
	printf("totalhit:%d\n", totalhit);

	return precision/testnum;
}
double AUC(double(*U)[D], double(*V)[D], double*bv,PREVIEW_ON_ITEM* rm_test, PREVIEW_ON_ITEM* rm_train,\
	int usernum, int itemnum, int type)
{
	int *checkitem = (int *)malloc(sizeof(int) * itemnum);
	int vid_i, vid_j;
	int uid;
	PREVIEW_ON_ITEM point_rating;
	double total_auc = 0;
	int test_usernum = 0;
	for (int i = 1; i < usernum; ++i)
	{
		// if this user does not exist in training set
		// skip this user
		if (rm_train[i]==NULL)
		{
			continue;
		}
		int hit=0;
		uid = i;
		// unrated_count is the number of items which is not rated by user
		// test_count is the number of items in test set
		int unrated_count=0;
		int test_count=0;
		// filter out the item in test and train sets
		for (int j = 1; j < itemnum; ++j)
		{
			checkitem[j] = 1;
		}
		point_rating = rm_train[i];
		while (point_rating)
		{
			checkitem[point_rating->vid] = 0;
			point_rating = point_rating->next;
			unrated_count++;
		}
		point_rating = rm_test[i];
		while (point_rating)
		{
			checkitem[point_rating->vid] = 0;
			point_rating = point_rating->next;
			test_count++;
			unrated_count++;
		}
		if (test_count == 0)
		{
			continue;
		}
		++test_usernum;
		// the corresponding value in checkitem of item i should be 1 if item i is not in training
		// and test set
		unrated_count = itemnum - unrated_count;

		// begin the calculation of auc
		point_rating = rm_test[i];
		// for each item in test set
		while (point_rating)
		{
			vid_i = point_rating->vid;
			for (int j = 1; j < itemnum; ++j)
			{
				if (checkitem[j])
				{
					vid_j = j;
					double score_i = 0;
					double score_j = 0;
					score_i = multiVector(U[uid],V[vid_i],D);
					score_j = multiVector(U[uid],V[vid_j],D);
					// for MF, we should add the bias factor
					if (type == MF)
					{
						score_i += bv[vid_i];
						score_j += bv[vid_j];
					}
					if (score_i>score_j)
					{
						hit++;
					}
				}
			}
			point_rating = point_rating->next;
		}

		total_auc += (double)hit/((double)test_count*(double)unrated_count);
		// printf("%f\n", total_auc);
		// getchar();
	}
	return total_auc/test_usernum;
}

double nDCG(RECOMM_NODE(*recomm_list)[_N],PREVIEW_ON_ITEM* rm_test, int usernum, int type)
{
	//for each user, compute TOP-K item list:
	int count = 0;
	double ndcg=0;
	PREVIEW_ON_ITEM point_rating;

	int* count_item_test = malloc(sizeof(int)*usernum);
	for (int i = 1; i < usernum; ++i)
	{
		count_item_test[i] = 0;
	}

	for (int i = 1; i < usernum; ++i)
	{
		point_rating = rm_test[i];
		while (point_rating)
		{
			++count_item_test[i];
			point_rating=point_rating->next;
		}
	}

	for (int uid = 0; uid < usernum; ++uid) {
		if (rm_test[uid] == NULL)
		{
			continue;
		};
		++count;

		double Zu=0;

		// the max DCG
		// rank the item in test set
		if (type == EXPLISIT)
		{
			RECOMM_NODE test_rank[_N];
			// initialize the test rank list
			for (int j = 0; j < _N; ++j)
			{
				test_rank[j].real_rating = 0;
				test_rank[j].vid = -1;
			}

			point_rating = rm_test[uid];
			while (point_rating)
			{
				// rank the item by evaluation
				for (int n = 0; n < _N; ++n)
				{
					if (point_rating->rating>test_rank[n].real_rating)
					{
						if (n==0)
						{
							test_rank[n].real_rating = point_rating->rating;
							test_rank[n].vid = point_rating->vid;
						}
						else
						{
							test_rank[n-1].real_rating = test_rank[n].real_rating;
							test_rank[n-1].vid = test_rank[n].vid;
							test_rank[n].real_rating = point_rating->rating;
							test_rank[n].vid = point_rating->vid;
						}
					}
				}
				point_rating=point_rating->next;
			}
		}
		else if (type == IMPLISIT)
		{
			int max_N;
			max_N = _N>count_item_test[uid]?_N:count_item_test[uid];
			for (int j = 0; j < max_N; ++j)
			{
				Zu += 1 / log2(j + 2);
			}
			point_rating = rm_test[uid];
			while (point_rating)
			{
				for (int n = 0; n < _N; ++n)
				{
					if (recomm_list[uid][n].vid == point_rating->vid)
					{
						ndcg += 1 / log2(_N-n + 1)/Zu;
						// printf("ndcg hit:%f\n",ndcg);
						break;
					}
				}
				point_rating = point_rating->next;
			}
		}
		else
		{
			printf("type error\n");
			getchar();
		}
	}
	ndcg /= count;
	free(count_item_test);
	return ndcg;
}

void makeRecommend_price(double(*U)[D],double(*V)[D],double*bu,double*bv,PREVIEW_ON_ITEM*rm_train,\
	int*trained_item, RECOMM_NODE(*recomm_list)[_N], int usernum, int itemnum, double total_mean,\
	USER_INFO* userlist, ITEM_INFO* itemlist, double penalty, int type)
{
	// initialize the recommend list
	for (int i = 1; i < usernum; ++i)
	{
		for (int j = 0; j < _N; ++j)
		{
			recomm_list[i][j].eval_rating = 0;
			recomm_list[i][j].real_rating = 0;
			recomm_list[i][j].vid = -1;
		}
	}

	// for each user
	for (int uid = 1; uid < usernum; ++uid)
	{
		if (DEBUG)
		{
			printf("%d\n", uid);
		}
		// used to filter out the items in training set
		int *filter_item = (int *)malloc(sizeof(int) * itemnum);
		for (int i = 0; i < itemnum; ++i)
		{
			filter_item[i] = 0;
		}
		PREVIEW_ON_ITEM point_rating;
		point_rating = rm_train[uid];
		while (point_rating)
		{
			filter_item[point_rating->vid] = 1;
			point_rating = point_rating->next;
		}

		// for each item
		for (int vid = 1; vid < itemnum; ++vid)
		{
			if (filter_item[vid])
			{
				continue;
			}
			// the evaluation uid's rating on vid
			double eval = 0;

			if (type == MF)
			{
				// first case, uid and vid all in training set
				if (rm_train[uid] && trained_item[vid]==1)
				{
					eval = multiVector(U[uid],V[vid],D);
					eval+=total_mean+bu[uid]+bv[vid];
				}
				else
				{
					eval = itemlist[vid].avg_rating;
				}
			}
			else if (type == BPR)
			{
				eval = multiVector(U[uid],V[vid],D);
			}

			if (userlist[uid].afford<itemlist[vid].price)
			{
				eval -= penalty;
			}
			// rank the item by evaluation
			for (int n = 0; n < _N; ++n)
			{
				if (eval>recomm_list[uid][n].eval_rating)
				{
					if (n==0)
					{
						recomm_list[uid][n].eval_rating = eval;
						recomm_list[uid][n].vid = vid;
					}
					else
					{
						recomm_list[uid][n-1].eval_rating = recomm_list[uid][n].eval_rating;
						recomm_list[uid][n-1].vid = recomm_list[uid][n].vid;
						recomm_list[uid][n].eval_rating = eval;
						recomm_list[uid][n].vid = vid;
					}
				}
			}
		}
	}
}
