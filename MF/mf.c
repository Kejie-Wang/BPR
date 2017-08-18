#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../Evaluation/recomm.h"
#include "mf.h"

extern int usernum, itemnum;
extern double train_mean;
void matrixFactorization(double (*U)[D],double (*V)[D],double* bu, double*bv, PREVIEW_ON_ITEM* rm_train, \
PREVIEW_ON_ITEM* rm_validation, double* item_mean, MFParam param)
{
	double Alpha = param.alpha;
	double Learn_loop= param.Learn_loop;
	double LambdaUV = param.lambdaUV;
	double LambdaBias = param.lambdaBias;
	int sw_reg = param.sw_reg;
    int sw_bias = param.sw_bias;
    double stop_condition= 10;
    
    for (int learn_loop = 0; learn_loop < Learn_loop; ++learn_loop)
	{
		double cost = 0;
		double err = 0;
		for (int i = 1; i < usernum; ++i)
		{
			PREVIEW_ON_ITEM point_rating;
			point_rating = rm_train[i];
			while (point_rating)
			{
				int uid;
				int vid;
				uid = i;
				vid = point_rating->vid;
				// U(i)*V(j)
				double eval = 0;
				for (int d = 0; d < D; ++d)
				{
					eval += U[uid][d]*V[vid][d];
				}
				//eval += train_mean;
				if (sw_bias)
				{
					eval += bu[uid]+bv[vid];
				}
				// U(i)*V(j)-R(i)(j)
				eval -= point_rating->rating;
				for (int d = 0; d < D; ++d)
				{
					U[uid][d] -= Alpha*eval*V[vid][d];
					V[vid][d] -= Alpha*eval*U[uid][d];
					if (sw_reg)
					{
						U[uid][d] -= LambdaUV*Alpha*U[uid][d];
						V[vid][d] -= LambdaUV*Alpha*V[vid][d];
					}
				}
				
				bu[uid] -= Alpha*eval;
				if (sw_reg)
				{
					bu[uid] -= Alpha*LambdaBias*bu[uid];
				}
				bv[vid] -= Alpha*eval;
				if (sw_reg)
				{
					bv[vid] -= Alpha*LambdaBias*bv[vid];
				}
				point_rating = point_rating->next;
			}
		}
		cost = rmse(U,V,bu,bv,item_mean,rm_train,rm_train,usernum,itemnum,train_mean);
		err = rmse(U,V,bu,bv,item_mean, rm_validation,rm_train,usernum,itemnum,train_mean);
		
		printf("Train rmse is: %f\n", cost);
		printf("Test rmse is: %f\n", err);

		if (err>stop_condition)
		{	
			printf("Stop train\n");
			break;
		}
		stop_condition = err;
		// printf("Iteration: %d \n", learn_loop);
		// printf("Train rmse is: %f\n", cost);
		// printf("Test rmse is: %f\n", err);
	}
}
