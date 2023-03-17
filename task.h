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

//任务队列，暂定让该任务队列的任务数固定，机器人取走任务后标记为busy
extern vector<TaskInfo> waiting_task_list;

//能被执行的任务队列
extern vector<int> avail_taskid_list;

//新增一个任务，不用排序
void addTaskInfo(int s, int d, int value, float distance, float weight);

//所有任务按照weight排序
void sortTaskList(void);

//查看任务是否已经在被处理了
bool isTaskBusy(int id);

//标记任务忙碌
void tagBusyTask(int id);

//标记任务空闲
void tagFreeTask(int id);

//获取source
int getSourceOfTask(int id);

//获取dest
int getDestOfTask(int id);

//将能执行的任务id加入taskid_list
void addTaskId(int id);

//清空taskId_list
void clearTaskIdList(void);

//获取任务固定权重
float getWeightOfTask(int id);

#endif