#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include "../GeneralStruct/gvStruct.h"
#include "../Evaluation/recomm.h"
#include "main.h"
#include "mf.h"
// #include "bpr.h"

int usernum, itemnum;
int sw_bias, sw_reg;

double train_mean;
int train_count = 0;
int validation_count = 0;
int test_count = 0;

int main(int argc, char const *argv[])
{
	// some switch
	// switch for bias
	sw_bias = 1;
	// switch for regularization
	sw_reg = 1;
	// execute the python script to generate a traing set and a testing set
	char* dataname = "yelp_dataset/yelp";
	char* cmd = malloc(sizeof(char)*256);
	strcpy(cmd, "python yelp_dataset/yelp_choose.py ");
	strcat(cmd, dataname);
	system(cmd);


	char* statfile = malloc(sizeof(char)*128);
	strcpy(statfile,dataname);
	strcat(statfile,".stat");

	char* trainfile = malloc(sizeof(char)*128);
	char* validationfile = malloc(sizeof(char) * 128);
	char* testfile = malloc(sizeof(char)*128);
	strcpy(trainfile,dataname);
	strcpy(validationfile, dataname);
	strcpy(testfile,dataname);
	strcat(trainfile,".train");
	strcat(validationfile, ".validation");
	strcat(testfile,".test");

	char* userfile = malloc(sizeof(char)*128);
	strcpy(userfile,dataname);
	strcat(userfile,"_user.info");
	char* itemfile = malloc(sizeof(char)*128);
	strcpy(itemfile,dataname);
	strcat(itemfile,"_item.info");

	// read in the user number and item number for setting up the array
	FILE *inputFile = fopen(statfile,"r");
	fscanf(inputFile,"%d %d %d %d %d",&usernum,&itemnum,&train_count,&validation_count, &test_count);
	fclose(inputFile);
	PREVIEW_ON_ITEM* rating_matrix_train;
	rating_matrix_train = malloc(sizeof(PREVIEW_ON_ITEM)*usernum);
	PREVIEW_ON_ITEM* rating_matrix_validation;
	rating_matrix_validation = malloc(sizeof(PREVIEW_ON_ITEM)*usernum);
	PREVIEW_ON_ITEM* rating_matrix_test;
	rating_matrix_test = malloc(sizeof(PREVIEW_ON_ITEM)*usernum);

	// initialize the rating matrix
	for (int i = 1; i < usernum; ++i)
	{
		rating_matrix_train[i] = NULL;
		rating_matrix_validation[i] = NULL;
		rating_matrix_test[i] = NULL;
	}
	// the train rating matrix
	PREVIEW_ON_ITEM* rm_train = initialRatingMatrix(trainfile, rating_matrix_train);
	// the validation rating matrix
	PREVIEW_ON_ITEM* rm_validation = initialRatingMatrix(validationfile, rating_matrix_validation);
	// the test rating matrix
	PREVIEW_ON_ITEM* rm_test = initialRatingMatrix(testfile, rating_matrix_test);

	USER_INFO* user_info = malloc(sizeof(USER_INFO)*usernum);
	ITEM_INFO* item_info = malloc(sizeof(ITEM_INFO)*itemnum);
	initialUserInfo(userfile,user_info);
	initialItemInfo(itemfile,item_info);

	// possible warning here, if the itemnum is very large, the stack of the program may be used up
	// initialize item_mean, for evaluate the item's rating for cold user
	// because we have no information of cold user, the only reasonable solution is to use item_mean
	double *item_mean = (double *)malloc(sizeof(double) * itemnum);
	short *item_count = (short *)malloc(sizeof(short) * itemnum);
	for (int i = 1; i < itemnum; ++i)
	{
		item_mean[i] = 0;
		item_count[i] = 0;
	}
	// get the item_mean from training set
	// some users may rate same item more than once, here get the item's average rating
	// get the train_mean, train_mean is used to evaluate cold item's rating, and also used to normalize
	// the rating matrix.
	for (int i = 1; i < usernum; ++i)
	{
		PREVIEW_ON_ITEM point_rating;
		point_rating = rm_train[i];
		while (point_rating)
		{
			train_mean += point_rating->rating;
			point_rating->rating /= point_rating->count;	//NOTE: using the mean to represent the rating
			item_mean[point_rating->vid]+= point_rating->rating;
			item_count[point_rating->vid]++;

			point_rating = point_rating->next;
		}
	}
	train_mean /= train_count;
	printf("total mean:%f\n", train_mean);
	for (int i = 1; i < itemnum; ++i)
	{
		if (item_count[i]!=0)
		{
			item_mean[i]/=item_count[i];
		}
		else
		{
			item_mean[i]=-1;
		}
	}

	//user_mean
	double* user_mean = (double*)(malloc(sizeof(double) * usernum));
	short user_count;
	for (int i = 0; i < usernum; ++i)
	{
		user_count = 0;
		PREVIEW_ON_ITEM point_rating;
		point_rating = rm_train[i];
		while (point_rating)
		{
			user_mean[i] += point_rating->rating;
			user_count++;
		}
		if (user_count == 0)
			user_mean[i] = -1;
		else
			user_mean[i] /= user_count;
	}


	//trained_item to represent whether a item has been rated before
	//0->no one has rated it
	//1->some one has rated it
	int* trained_item = malloc(sizeof(int)*itemnum);
	for (int i = 1; i < itemnum; ++i)
	{
		trained_item[i] = 0;
	}
	PREVIEW_ON_ITEM point_rating;
	for (int i = 1; i < usernum; ++i)
	{
		point_rating = rm_train[i];
		while (point_rating)
		{
			trained_item[point_rating->vid] = 1;
			point_rating = point_rating->next;
		}
	}

	// the model matrix
	double (*U)[D];
	double (*V)[D];
	double *bu;
	double *bv;

	U = malloc(sizeof(double)*D*usernum);
	V = malloc(sizeof(double)*D*itemnum);
	bu = malloc(sizeof(double)*usernum);
	bv = malloc(sizeof(double)*itemnum);

	// the recommender array
	RECOMM_NODE (*recomm_list)[_N];
	recomm_list = malloc(sizeof(RECOMM_NODE)*usernum*_N);

	double penalty[] = {0,0.1,0.5,1,2,3,4};

	for (int iter_test = 0; iter_test < TESTLOOP; ++iter_test)
	{
		// initial U, V, bu, bv
		initialFeatureVector(U,usernum);
		initialFeatureVector(V,itemnum);
		initialBiasVector(bu,usernum);
		initialBiasVector(bv,itemnum);

		printf("start mf here\n");
		// do mf here
		matrixFactorization(U,V,bu,bv,rm_train,rm_validation,item_mean);

		// rank all items for each user, and choose the top _N items as the recommender list
		// makeRecommend(U,V,bu,bv,rm_train,item_mean,recomm_list,usernum,itemnum,train_mean);

		// double auc;
		// auc = AUC(U, V, bv, rm_test,  rm_train, usernum, itemnum, MF);
		// printf("auc:%f\n", auc);


		// for (int iter_pen = 0; iter_pen < 7; ++iter_pen)
		// {
		// 	double tmp_penalty;
		// 	tmp_penalty = penalty[iter_pen];
			makeRecommend(U,V,bu,bv,rm_train,item_mean,recomm_list,\
				usernum,itemnum,train_mean);
			// makeRecommend_price(U,V,bu,bv,rm_train,trained_item,recomm_list,\
			// 	usernum,itemnum,train_mean,user_info,item_info,tmp_penalty,MF);
			printf("make recommden done\n");
			double precision;
			// calculate the precision here
			precision = Precision(recomm_list,rm_test,usernum);
			printf("precision: %f\n", precision);
			// printf("penalty:%f, precision:%f\n", tmp_penalty, precision);
			// double ndcg;
			// ndcg = nDCG(recomm_list,rm_test, usernum, IMPLISIT);
			// printf("NDCG:%f\n", ndcg);
			// the precision of the recommender list
		// }
	}

	return 0;
}

void debugRatingMatrix(PREVIEW_ON_ITEM* rm)
{
	// debug for filling rating matrix
	PREVIEW_ON_ITEM point_cursor;
	for (int i = 0; i < usernum; ++i)
	{
		for (int j = 0; j < itemnum; ++j)
		{
			point_cursor = rm[i];
			int flag_I = 0;
			while (point_cursor)
			{
				// printf("%d\n", point_cursor);
				if (point_cursor->vid == j)
				{
					flag_I = 1;
					printf("%f ", point_cursor->rating);
					break;
				}
				point_cursor = point_cursor->next;
			}
			if (!flag_I)
			{
				printf("0 ");
			}
		}
		printf("\n");
	}
}

void initialBiasVector(double* biasV, int num)
{
	srand((unsigned)time(NULL));
	for (int i = 1; i < num; ++i)
	{
		biasV[i] = (double)(rand()%1000000)/1000000;
	}
}
