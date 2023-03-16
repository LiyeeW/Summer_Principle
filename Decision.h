#ifndef decision_h
#define decision_h

//选择类型的数量
const int DECISION_TYPE_NUM = 5;



//选择信息
typedef struct {
    int type;       //0-4的选择类型
    int robot;      //机器人ID
    float param;    //参数2
} DecisionInfo;

//选择的数量
extern int decision_num;


//选择类型对应的字符串
extern char decision_string[DECISION_TYPE_NUM][10];
//选择信息表
extern DecisionInfo decision_info_table[50];

//做出选择
void makeDecision();
//输出选择
void outputDecision();

//地图初始化：工作台联系表 和 任务队列
void initMap(void);

//计算具有最大price的任务并返回
void assignTaskfromBids();

//遍历所有任务，找出能前往的任务id放到avail_taskid_list
void updateAvailList(void);

//为robot_id生成报价并排序
void generateBids(int robot_id);

#endif