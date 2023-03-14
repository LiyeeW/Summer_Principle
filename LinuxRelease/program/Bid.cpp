#include"Bid.h"

//R*T的报价表
vector<BidInfo> bids_list;

//加入报价
void addBidInfo(int r,int t,float price){
    bids_list.push_back({r,t,price});
}

bool bidcompare(BidInfo b1,BidInfo b2){
    return b1.price>b2.price;
}

//对全部报价按price排序
void sortBidList(void){
    sort(bids_list.begin(),bids_list.end(),bidcompare);
}