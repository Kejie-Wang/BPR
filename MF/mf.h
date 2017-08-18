#ifndef _MF_H_
#define _MF_H_

#include "../GeneralStruct/gvStruct.h"

void matrixFactorization(double(*)[D],double(*)[D],double*,double*,PREVIEW_ON_ITEM*,PREVIEW_ON_ITEM*,double*, MFParam);

// double rmse(double(*)[D],double(*)[D],double*bu,double*bv,double*,PREVIEW_ON_ITEM* ,PREVIEW_ON_ITEM* );

// void makeRecommend(double(*)[D],double(*)[D],double*,double*,\
// 	PREVIEW_ON_ITEM*,double*,int(*)[_N],double(*)[_N]);

// double Precision(int(*)[_N],PREVIEW_ON_ITEM*);

double pow(double x, double y);

#endif
