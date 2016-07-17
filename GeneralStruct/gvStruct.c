#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gvStruct.h"

PREVIEW_ON_ITEM* initialRatingMatrix(char* filename, PREVIEW_ON_ITEM* rating_matrix)
{
	FILE *inputFile = fopen(filename,"r");
	int uid, vid;
	float rating;
	// fill the rating matrix
	while (!feof(inputFile))
	{
		// read each line of the file
		fscanf(inputFile,"%d %d %f\n",&uid,&vid,&rating);
		if (rating_matrix[uid])
		{
			PREVIEW_ON_ITEM point_cursor;
			point_cursor = rating_matrix[uid];
			// flag_Dup is used to decide whether user has rated one item more than once
			int flag_Dup = 0;
			do
			{
				if (point_cursor->vid == vid)
				{
					// printf("001 warning: user %d has rated item %d twice\n",uid,vid);
					// printf("previous rating:%d, now rating:%hd \n", point_cursor->rating,rating);
					point_cursor->rating += rating;
					(point_cursor->count)++;
					flag_Dup = 1;
					break;
				}
				if (point_cursor->next)
				{
					point_cursor = point_cursor->next;
				}
				else
				{
					break;
				}
			} while (1);
			if (!flag_Dup)
			{
				PREVIEW_ON_ITEM record = (PREVIEW_ON_ITEM)malloc(sizeof(REVIEW_ON_ITEM));
				record->next = NULL;
				record->vid = vid;
				record->rating = rating;
				record->count = 1;
				point_cursor->next = record;
			}
		}
		else
		{
			rating_matrix[uid] = (PREVIEW_ON_ITEM)malloc(sizeof(REVIEW_ON_ITEM));
			rating_matrix[uid]->next = NULL;
			rating_matrix[uid]->vid = vid;
			rating_matrix[uid]->rating = rating;
			rating_matrix[uid]->count = 1;
		}
	}
	fclose(inputFile);
	
	return rating_matrix;
}

void initialFeatureVector(double (*featureV)[D], int num)
{
	srand((unsigned)time(NULL));
	for (int i = 1; i < num; ++i)
	{
		for (int j = 0; j < D; ++j)
		{
			featureV[i][j] = (double)(rand()%1000000)/1000000;
		}
	}
}

double multiVector(double* v1, double *v2, int d)
{
	double result = 0;
	for (int i = 0; i < d; ++i)
	{
		result+=v1[i]*v2[i];
	}
	return result;
}

void initialUserInfo(char* filename, USER_INFO* userlist)
{
	FILE *inputFile = fopen(filename,"r");
	int uid;
	float avg_rating;
	float afford;

	// fill the rating matrix
	while (!feof(inputFile))
	{
		fscanf(inputFile,"%d %f %f",&uid,&afford,&avg_rating);
		userlist[uid].afford = afford;
		userlist[uid].avg_rating = avg_rating;
	}
}

void initialItemInfo(char* filename, ITEM_INFO* itemlist)
{
	FILE *inputFile = fopen(filename,"r");
	int vid;
	float avg_rating;
	float price;

	// fill the rating matrix
	while (!feof(inputFile))
	{
		fscanf(inputFile,"%d %f %f",&vid,&price,&avg_rating);
		itemlist[vid].price = price;
		itemlist[vid].avg_rating = avg_rating;
	}
}

void leaveOneOut(PREVIEW_ON_ITEM* test, PREVIEW_ON_ITEM* all, int usernum)
{
	PREVIEW_ON_ITEM point_rating;

	for (int i = 1; i < usernum; ++i)
	{
		int rated_item_count = 0;
		int random_choose;
		point_rating = all[i];
		while (point_rating)
		{
			point_rating->rating /= point_rating->count;
			rated_item_count ++;
			point_rating = point_rating->next;
		}
		if (rated_item_count<2)
		{
			continue;
		}
		srand((unsigned)time(NULL));
		random_choose = rand()%rated_item_count;
		rated_item_count = 0;
		point_rating = all[i];
		PREVIEW_ON_ITEM pre_point_rating;
		while (point_rating)
		{
			if (rated_item_count == random_choose)
			{
				// get one node out of the chain
				if (rated_item_count == 0)
				{
					// the head node
					all[i] = point_rating->next;
					point_rating->next=NULL;
					test[i] = point_rating;
					break;
				}
				else
				{
					//pre_point_rating->next=point_rating->next;
					point_rating->next=NULL;
					test[i] = point_rating;
					break;
				}

			}
			rated_item_count++;
			pre_point_rating = point_rating;
			point_rating=point_rating->next;
		}
	}
}