#ifndef task_h
#define task_h

#include<vector>
#include<algorithm>

using namespace std;

//最大任务数量
extern int task_num;

//任务模板
typedef struct{
    int id;
    int source;
    int dest;
    int value;
    float distance;  
    float weight; //该任务权重，目前设定为：S的剩余生产时间+d的空出时间+差价/(机器人到生产工作台的距离+生产工作台到收购工作台的距离)
    bool busy;
}TaskInfo;

//任务队列，暂定让该任务队列的任务数固定，机器人取走任务后直到完成，任务才重新回到队列中
extern vector<TaskInfo> waiting_task_list;

//

//新增一个任务，不用排序
void addTaskInfo(int s, int d, int value, float distance, float weight);


//所有任务按照weight排序
void sortTaskList(void);

#endif