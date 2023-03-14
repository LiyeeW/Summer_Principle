#ifndef bid_h
#define bid_h

#include<vector>
#include<algorithm>

using namespace std;


//报价结构
typedef struct{
    int robot_id;
    int task_id;
    float price; //robot对该任务的报价
}BidInfo;

//R*T的报价表
extern vector<BidInfo> bids_list;

//加入报价
void addBidInfo(int r,int t,float price);

//对全部报价按price排序
void sortBidList(void);

#endif