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

#endif