#include "task.h"

//任务队列
list<TaskInfo> waiting_task_list;

//最大任务数量
int task_num;

//新增一个任务，不用排序
void addTaskInfo(int s, int d, float weight){
    waiting_task_list.push_back({s,d,weight});
    task_num++;
}

//所有任务按照优先级排序
void sortTaskList(void);