#ifndef _RECOMM_H_
#define _RECOMM_H_

#include "../GeneralStruct/gvStruct.h"

double pow(double x, double y);
#ifndef FRMSE
#define FRMSE
double rmse(double(*U)[D],double(*V)[D],double*bu,double*bv,double*item_mean,PREVIEW_ON_ITEM* \
	rm_test,PREVIEW_ON_ITEM* rm_train, int usernum, int itemnum,double total_mean);
#endif

#ifndef FRMSEIMPLICIT
#define FRMSEIMPLICIT
double rmse_implicit(double(*U)[D], double(*V)[D], double*bu, double*bv, double*item_mean, PREVIEW_ON_ITEM* \
	rm_test, PREVIEW_ON_ITEM* rm_train, int usernum, int itemnum, double total_mean);
#endif

#ifndef FMAKERECOMMEND
#define FMAKERECOMMEND
void makeRecommend(double(*U)[D],double(*V)[D],double*bu,double*bv,PREVIEW_ON_ITEM*rm_train,\
	double*item_mean, RECOMM_NODE(*recomm_list)[MAXN], int usernum, int itemnum,double total_mean);
#endif

#ifndef FPRECISIONANDRECALL
#define FPRECISIONANDRECALL
void PrecisionAndRecall(RECOMM_NODE(*recomm_list)[MAXN],PREVIEW_ON_ITEM* rm_test, int usernum, double* precision, double* recall, int N);
#endif


#ifndef FNDCG
#define FNDCG
double nDCG(RECOMM_NODE(*recomm_list)[MAXN],PREVIEW_ON_ITEM* rm_test, int usernum, int type);
#endif

#ifndef FAUC
#define FAUC
double AUC(double(*U)[D], double(*V)[D], double* bv, PREVIEW_ON_ITEM* rm_test, PREVIEW_ON_ITEM* rm_train,\
	int usernum, int itemnum, int type);
#endif

#ifndef FMAKERECOMMENDPRICE
#define FMAKERECOMMENDPRICE
void makeRecommend_price(double(*U)[D],double(*V)[D],double*bu,double*bv,PREVIEW_ON_ITEM*rm_train,\
	int*trained_item, RECOMM_NODE(*recomm_list)[MAXN], int usernum, int itemnum, double total_mean,\
	USER_INFO* userlist, ITEM_INFO* itemlist, double penalty, int type);
#endif


#endif
