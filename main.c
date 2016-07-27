#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "GeneralStruct/gvStruct.h"
#include "Evaluation/recomm.h"
#include "main.h"
#include "mf/mf.h"
#include "BPR\bpr.h"

int usernum, itemnum;
int sw_bias, sw_reg;

int train_count = 0;
int validation_count = 0;
int test_count = 0;
double train_mean = 0;

void datasetRead(char* dataname, PREVIEW_ON_ITEM* rm_train, PREVIEW_ON_ITEM* rm_validation, PREVIEW_ON_ITEM* rm_test, \
	USER_INFO* user_info, ITEM_INFO* item_info, double* item_mean)
{
	char* trainfile = malloc(sizeof(char) * 128);
	char* validationfile = malloc(sizeof(char) * 128);
	char* testfile = malloc(sizeof(char) * 128);
	strcpy(trainfile, dataname);
	strcpy(validationfile, dataname);
	strcpy(testfile, dataname);
	strcat(trainfile, ".train");
	strcat(validationfile, ".validation");
	strcat(testfile, ".test");

	/*char* userfile = malloc(sizeof(char) * 128);
	strcpy(userfile, dataname);
	strcat(userfile, "_user.info");
	char* itemfile = malloc(sizeof(char) * 128);
	strcpy(itemfile, dataname);
	strcat(itemfile, "_item.info");*/

	// the train rating matrix
	initialRatingMatrix(trainfile, rm_train);
	// the validation rating matrix
	initialRatingMatrix(validationfile, rm_validation);
	// the test rating matrix
	initialRatingMatrix(testfile, rm_test);

	free(trainfile);
	free(validationfile);
	free(testfile);

	/*initialUserInfo(userfile, user_info);
	initialItemInfo(itemfile, item_info);*/

	// possible warning here, if the itemnum is very large, the stack of the program may be used up
	// initialize item_mean, for evaluate the item's rating for cold user
	// because we have no information of cold user, the only reasonable solution is to use item_mean
	
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
	train_mean = 0;
	for (int i = 1; i < usernum; ++i)
	{
		PREVIEW_ON_ITEM point_rating;
		point_rating = rm_train[i];
		while (point_rating)
		{
			train_mean += point_rating->rating;
			point_rating->rating /= point_rating->count;	//NOTE: using the mean to represent the rating
			item_mean[point_rating->vid] += point_rating->rating;
			item_count[point_rating->vid]++;

			point_rating = point_rating->next;
		}
	}
	train_mean /= train_count;
	printf("total mean:%f\n", train_mean);
	for (int i = 1; i < itemnum; ++i)
	{
		if (item_count[i] != 0)
		{
			item_mean[i] /= item_count[i];
		}
		else
		{
			item_mean[i] = -1;
		}
	}

	free(item_count);

	//user_mean
	/*double* user_mean = (double*)(malloc(sizeof(double) * usernum));
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
	point_rating = point_rating->next;

	}
	if (user_count == 0)
	user_mean[i] = -1;
	else
	user_mean[i] /= user_count;
	}*/

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
}

void mf_recomm(PREVIEW_ON_ITEM* rm_train, PREVIEW_ON_ITEM* rm_validation, PREVIEW_ON_ITEM* rm_test, double *item_mean, double train_mean, \
	double(*U)[D], double(*V)[D], double *bu, double *bv, RECOMM_NODE(*recomm_list)[MAXN], MFParam param, char *filename)
{
	// initial U, V, bu, bv
	initialFeatureVector(U, usernum);
	initialFeatureVector(V, itemnum);
	initialBiasVector(bu, usernum);
	initialBiasVector(bv, itemnum);

	printf("start mf here\n");
	// do mf here
	matrixFactorization(U, V, bu, bv, rm_train, rm_validation, item_mean, param);

	printf("stop mf here\n");

	double test_rmse = rmse(U, V, bu, bv, item_mean, rm_test, rm_train, usernum, itemnum, train_mean);
	printf("Test rmse is: %f\n", test_rmse);
	// rank all items for each user, and choose the top _N items as the recommender list
	// makeRecommend(U,V,bu,bv,rm_train,item_mean,recomm_list,usernum,itemnum,train_mean);

	printf("make recommendation here\n");
	makeRecommend(U, V, bu, bv, rm_train, item_mean, recomm_list, \
		usernum, itemnum, train_mean);
	// makeRecommend_price(U,V,bu,bv,rm_train,trained_item,recomm_list,\
						// 	usernum,itemnum,train_mean,user_info,item_info,tmp_penalty,MF);
	printf("make recommendation done\n");
//	saveRecommList(recomm_list, filename, usernum);
}

void bpr_recomm(PREVIEW_ON_ITEM* rm_train, PREVIEW_ON_ITEM* rm_validation, PREVIEW_ON_ITEM* rm_test, double *item_mean, double train_mean, \
	double(*U)[D], double(*V)[D], RECOMM_NODE(*recomm_list)[MAXN], BPRParam param)
{
	initialFeatureVector(U, usernum);
	initialFeatureVector(V, itemnum);

	printf("start bpr here");

	//do bpr here
	bpr(U, V, rm_train, rm_validation, param);
	
	printf("bpr done\n");

	printf("make recommendation here\n");
	makeRecommend(U, V, NULL, NULL, rm_train, rm_test, recomm_list, usernum, itemnum, train_mean);
	printf("make recommendation done\n");

}

void getMfRecommListFileName(char* filename, MFParam param)
{
	sprintf(filename, "alpha=%f,Loop=%f,lambdaUV=%f,lambdaBias=%f,reg=,bias=",
		param.alpha, param.Learn_loop, param.lambdaUV, param.lambdaBias, param.sw_reg, param.sw_bias);
}

int setMFParams(MFParam* mfPrams)
{
	mfPrams[0].alpha = 0.008;
	mfPrams[0].Learn_loop = 500;
	mfPrams[0].lambdaUV = 0.12;
	mfPrams[0].lambdaBias = 0.12;
	mfPrams[0].sw_reg = 1;
	mfPrams[0].sw_bias = 1;

	mfPrams[1].alpha = 0.0085;
	mfPrams[1].Learn_loop = 500;
	mfPrams[1].lambdaUV = 0.12;
	mfPrams[1].lambdaBias = 0.12;
	mfPrams[1].sw_reg = 1;
	mfPrams[1].sw_bias = 1;

	mfPrams[2].alpha = 0.0085;
	mfPrams[2].Learn_loop = 500;
	mfPrams[2].lambdaUV = 0.12;
	mfPrams[2].lambdaBias = 0.123;
	mfPrams[2].sw_reg = 1;
	mfPrams[2].sw_bias = 1;

	mfPrams[3].alpha = 0.0085;
	mfPrams[3].Learn_loop = 500;
	mfPrams[3].lambdaUV = 0.12;
	mfPrams[3].lambdaBias = 0.127;
	mfPrams[3].sw_reg = 1;
	mfPrams[3].sw_bias = 1;

	mfPrams[4].alpha = 0.0085;
	mfPrams[4].Learn_loop = 500;
	mfPrams[4].lambdaUV = 0.123;
	mfPrams[4].lambdaBias = 0.123;
	mfPrams[4].sw_reg = 1;
	mfPrams[4].sw_bias = 1;

	mfPrams[5].alpha = 0.0085;
	mfPrams[5].Learn_loop = 500;
	mfPrams[5].lambdaUV = 0.127;
	mfPrams[5].lambdaBias = 0.123;
	mfPrams[5].sw_reg = 1;
	mfPrams[5].sw_bias = 1;

	mfPrams[6].alpha = 0.0085;
	mfPrams[6].Learn_loop = 500;
	mfPrams[6].lambdaUV = 0.123;
	mfPrams[6].lambdaBias = 0.123;
	mfPrams[6].sw_reg = 1;
	mfPrams[6].sw_bias = 1;

	mfPrams[7].alpha = 0.009;
	mfPrams[7].Learn_loop = 500;
	mfPrams[7].lambdaUV = 0.123;
	mfPrams[7].lambdaBias = 0.123;
	mfPrams[7].sw_reg = 1;
	mfPrams[7].sw_bias = 1;

	return 8;
}

void printMFParams(MFParam param)
{
	fprintf(stdout, "-----------------------------------------\n");
	fprintf(stdout, "alpha = %f, lambdaUV=%f, lambdaBias=%f, sw_reg=%d, sw_bias=%d\n",
		param.alpha, param.lambdaUV, param.lambdaBias, param.sw_reg, param.sw_bias);
}

int setBPRParams(BPRParam* bprParams)
{
	bprParams[0].alpha = 0.01;
	bprParams[0].lambda = 0.01;

	return 1;
}

void printBPRParams(BPRParam param)
{
	fprintf(stdout, "-----------------------------------------\n");
	fprintf(stdout, "alpha=%f, lambda=%f\n", param.alpha, param.lambda);
}

int main(int argc, char const *argv[])
{
	//read the data set into the matrix
	char* dataname = "amazon_dataset/amazon_mixture";

	// execute the python script to generate a traing set and a testing set
	char* cmd = malloc(sizeof(char) * 256);
	strcpy(cmd, "python amazon_dataset/amazon_choose.py ");
	strcat(cmd, dataname);
	//system(cmd);
	free(cmd);

	// read in the user number and item number for setting up the array
	char* statfile = malloc(sizeof(char) * 128);
	strcpy(statfile, dataname);
	strcat(statfile, ".stat");
	FILE *inputFile = fopen(statfile, "r");
	fscanf(inputFile, "%d %d %d %d %d", &usernum, &itemnum, &train_count, &validation_count, &test_count);
	fclose(inputFile);
	free(statfile);

	PREVIEW_ON_ITEM* rm_train = malloc(sizeof(PREVIEW_ON_ITEM)*usernum);
	PREVIEW_ON_ITEM* rm_validation = malloc(sizeof(PREVIEW_ON_ITEM)*usernum);
	PREVIEW_ON_ITEM* rm_test = malloc(sizeof(PREVIEW_ON_ITEM)*usernum);

	USER_INFO* user_info = malloc(sizeof(USER_INFO)*usernum);
	ITEM_INFO* item_info = malloc(sizeof(ITEM_INFO)*itemnum);

	double *item_mean = (double *)malloc(sizeof(double) * itemnum);
	// initialize the rating matrix
	for (int i = 0; i < usernum; ++i)
	{
		rm_train[i] = NULL;
		rm_validation[i] = NULL;
		rm_test[i] = NULL;
	} 
	datasetRead(dataname, rm_train, rm_validation, rm_test, user_info, item_info, item_mean);

	
	// the model matrix
	double(*U)[D];
	double(*V)[D];
	double *bu;
	double *bv;

	U = malloc(sizeof(double)*D*usernum);
	V = malloc(sizeof(double)*D*itemnum);
	bu = malloc(sizeof(double)*usernum);
	bv = malloc(sizeof(double)*itemnum);

	//Matrix Factorization algorithms test
	MFParam* mfParams = (MFParam*)(malloc(sizeof(MFParam) * 10));
	int mfParamsNum = setMFParams(mfParams);
	RECOMM_NODE(*recomm_list)[MAXN];
	recomm_list = malloc(sizeof(RECOMM_NODE)*usernum*MAXN);

	for (int i = 0; i < mfParamsNum; ++i)
	{
		printMFParams(mfParams[i]);
		memset(recomm_list, 0, sizeof(RECOMM_NODE)*usernum*MAXN);
		char *filename = (char*)malloc(sizeof(char) * 128);
		//if (loadRecommList(recomm_list, filename, usernum) == -1)
			mf_recomm(rm_train, rm_validation, rm_test, item_mean, train_mean, U, V, bu, bv, recomm_list, mfParams[i], filename);

		// calculate the precision and recall
		double precision, recall;
		int Ns[] = { 2, 5, 10, 20, 40, 60, 80, 100 };
		for (int i = 0; i < 8; ++i)
		{
			PrecisionAndRecall(recomm_list, rm_test, usernum, &precision, &recall, Ns[i]);
			printf("N=%d, precision: %f, recall: %f\n", Ns[i], precision, recall);
		}

		/*double auc;
		auc = AUC(U, V, bv, rm_test, rm_train, usernum, itemnum, MF);
		printf("auc:%f\n", auc);*/
		//calculate the ndcg
		/*double ndcg;
		ndcg = nDCG(recomm_list, rm_test, usernum, IMPLISIT);
		printf("NDCG:%f\n", ndcg);*/
	}
	

	//bpr algorithms test
	BPRParam* bprParams = (BPRParam*)(malloc(sizeof(BPRParam) * 10));
	int bprParamsNum = setBPRParams(bprParams);
	for (int i = 0; i < bprParams; ++i)
	{
		printBPRParams(bprParams[i]);
		memset(recomm_list, 0, sizeof(RECOMM_NODE)*usernum*MAXN);
		bpr_recomm(rm_train, rm_validation, rm_test, item_mean, train_mean, U, V, recomm_list, bprParams[i]);

		// calculate the precision and recall
		double precision, recall;
		PrecisionAndRecall(recomm_list, rm_test, usernum, &precision, &recall, MAXN);
		printf("precision: %f, recall: %f\n", precision, recall);

		double auc;
		auc = AUC(U, V, bv, rm_test, rm_train, usernum, itemnum, BPR);
		printf("auc:%f\n", auc);
	}

	system("pause");
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
