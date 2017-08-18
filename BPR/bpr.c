#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "../Evaluation/recomm.h"
#include "bpr.h"

extern int train_count;
extern int usernum, itemnum;

void bpr(double (*U)[D],double (*V)[D], PREVIEW_ON_ITEM* rm_train, PREVIEW_ON_ITEM* rm_validation, BPRParam param)
{
	// draw uid item_i, item_j
	int uid, vid_i, vid_j, choose_vid_i;

	// double rating_i, rating_j;
	double alpha = param.alpha;
	double lambda = param.lambda;

	int sample_count = 0, random_count, iteration=1;
	double stop_condition;
	PREVIEW_ON_ITEM point_rating;

	srand((unsigned)time(NULL));

	while(1)
	{
		// random choose the uid, vid_i, vid_j
		uid = rand()%(usernum-1)+1;

		// random choose vid_i, a rated item
		// count the total items rated by user i
		random_count = 0;
		point_rating = rm_train[uid];
		while (point_rating)
		{
			random_count++;
			point_rating = point_rating->next;
		}
		if (random_count == 0)
		{
			// no record of user i
			continue;
		}
		choose_vid_i = rand()%random_count;
		random_count = 0;

		point_rating = rm_train[uid];
		while (point_rating)
		{
			if (random_count == choose_vid_i)
			{
				vid_i = point_rating->vid;
			}
			point_rating = point_rating->next;
			random_count++;
		}

		// random choose a item which not in training set
		int notInTrain_flag = 0;
		do
		{
			notInTrain_flag = 0;
			vid_j = rand()%(itemnum-1)+1;
			point_rating = rm_train[uid];
			while (point_rating)
			{
				if (point_rating->vid == vid_j)
				{
					notInTrain_flag = 1;
				}
				random_count++;
				point_rating = point_rating->next;
			}			
		} while (notInTrain_flag);

		// printf("uid:%d,vid_i:%d,vid_j:%d\n",uid,vid_i,vid_j);
		// getchar();

		double diff=0;
		double tmp_sigmoid;
		for (int d = 0; d < D; ++d)
		{
			diff+=(U[uid][d]*V[vid_i][d]-U[uid][d]*V[vid_j][d]);
		}
		tmp_sigmoid = 1/(1+exp(-diff));
		tmp_sigmoid = 1-tmp_sigmoid;
		for (int d = 0; d < D; ++d)
		{
			U[uid][d] += alpha*(tmp_sigmoid*(V[vid_i][d]-V[vid_j][d])-lambda*U[uid][d]);
		}
		for (int d = 0; d < D; ++d)
		{
			V[vid_i][d] += alpha*(tmp_sigmoid*U[uid][d]-lambda*V[vid_i][d]);
			V[vid_j][d] -= alpha*(tmp_sigmoid*U[uid][d]+lambda*V[vid_j][d]);
		}
	
		sample_count ++;
		if (sample_count==train_count*10)
		{
			// check converge
			sample_count = 0;
			double auc;
			auc = AUC(U,V,NULL,rm_validation,rm_train,usernum,itemnum,BPR);
			if (iteration == 1)
			{
				stop_condition = auc;
			}
			else
			{
				if (auc<stop_condition)
				{
					printf("stop auc: %f iteration:%d\n",auc,iteration);
					break;
				}
				stop_condition = auc;
			}
			iteration++;
		}
	}
}
