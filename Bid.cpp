#include"Bid.h"

//R*T的报价表
vector<vector<BidInfo>> bids_list(4);

//加入报价
void addBidInfo(int r,int t,float price){
    bids_list[r].push_back({t,price});
}

bool bidcompare(BidInfo b1,BidInfo b2){
    return b1.price>b2.price;
}

//对robot_id的全部报价按price排序
void sortBidList(int r){
    sort(bids_list[r].begin(),bids_list[r].end(),bidcompare);
}

//清空robot_id的报价表
void clearBidList(int r){
    bids_list[r].clear();
}