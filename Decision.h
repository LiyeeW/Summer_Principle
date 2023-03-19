#ifndef decision_h
#define decision_h


//做出决策
void makeDecision();

//地图初始化：工作台联系表 和 任务队列
void initMap(void);

//计算具有最大price的任务并返回
void assignTaskfromBids();

//遍历所有任务，找出能前往的任务id放到avail_taskid_list
void updateAvailList(void);

//为robot_id生成报价并排序
void generateBids(int robot_id);

#endif