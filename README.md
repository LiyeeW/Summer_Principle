## 一、自底向上的模块介绍

1. Score
   1. 依赖：无
   2. 功能：
      1. 帧序号
      2. 分数
2. Station
   1. 依赖：无
   2. 功能：
      1. 单台基本信息
      2. 台间距离
      3. 台间联系
3. Task
   1. 依赖：暂无
      1. 信息上已依赖Station
   2. 功能：
      1. 任务基本信息
      2. 任务表排序
4. Robot
   1. 依赖：暂无
      1. 信息上已依赖Task
   2. 功能：
      1. 单人信息
5. Bid：
   1. 依赖：暂无
      1. 信息上依赖Task
   2. 功能：
      1. 任务的竞价信息
      2. 竞价排序
6. Decision
   1. 依赖：Score、Station、Task、Robot、Bid
   2. 功能：
      1. 根据运动系统，完成decision的输出

## 二、初步决策系统
前提：初始时获取所有任务并编号，放入waitint_task_list。若某一任务被分配后，标志为busy，以避免该任务被重复分配。

决策由空闲机器人触发，流程如下：

1. 如果有任务已完成，将该任务状态标志为free
2. 遍历所有任务，将满足条件1)发货工作台已有产品或处于生产中；2)收获工作台当前对应的原材料格为空，的所有任务加入avail_task_list中
3. 所有空闲机器人（R个）对avail_task_list的所有任务（T个）生成报价，报价=机器人到发货工作台的距离+差价/两工作台的距离，即R个大小为T的数组，对这T个数组分别从大到小排序
4. 分配任务，依次选取报价最高的任务分配给对应的robot


## debug记录 by wly
1. 计算机器人到目标位置的角度计算只考虑了-0.5PI到0-0.5PI
2. 计算角度差忘记把范围保留在-PI到PI
3. 积分部分没有限制，直接把PI拉满了
4. 到达且不用等待，写成了到达或不用等待
5. 等待条件搞反了
6. *收到了-1的状态任务
7. 初始运动阶段设为2
8. orientOffset计算没有考虑好突变
9. 调整角度PID参数，使得最初的角速度临近PI
10. 将开始线速度加速的差角临界值设为0.8，不算高，因为弧线加速需要考虑更多的周围的墙壁障碍物，所以尽可能先转方向再直线加速
11. 在stage 1，一旦检测到角度偏离，则跳转到stage 2
12. *计算差角的大小判断时，没有浮点精度判断
13. 阶段一有越过的可能，所以如果是绝对值接近PI的也不跳转
14. 阶段一回到阶段二之前，需要把反向标志清空
15. 判断是否超过时，考虑到浮点精度用到了差值的绝对值，应该套两层abs
16. 在switch里新增运动信息的更新
17. 新增状态4，相隔1.5m等待，减少被撞的概率
18. 放弃使用状态四，因为现有运动系统只能停在工作站附近。换用预估均速到达的方式，减短堵在工作站的时间
19. 针对送货也需要等待，修改wait的初始值均为true
20. 将movement较上层的部分抽离为moveStage，便于后续增加避障MoveTrace

1. getSpeed需要结合orient，有正负之分
2. 忘记switchConfType时setConfSolving了
3. 当到达目的地，partner也要回到regular
4. 平角同向：当双方之距离远大于某点到目的地的距离，则不处理


## trace更新修改记录 by wly
1. 修改原有的呆的角度pid，不仅支持朝向目标工作站，也支持朝向指定角度



# debug记录 by xy
1. [INFO]../Robot/Workbench.cpp:88|Sell|Product:3 already exists.

意思：目的地原材料格已满，东西卖不出去

原因：每一帧的执行流程是，先执行交易指令再做决策，即executeTrade->makeDecision，导致stionInfo.raw滞后更新，从而导致错误决策

解决方案：executeTrade后预更新stionInfo.raw

2. 程序运行错误，segmentation core dumped

原因：在生成任务队列时，为了避免等待，筛选了一些冲突任务，导致任务队列有可能为空，再次访问时造成非法访问

解决方案：放松了任务冲突条件，但会有大量的|Sell|Product:2 already exists.或|Buy|Robot already get product:2等错误，所以在Excute.cpp::updateRobotDestWait函数中添加了等待收货工作台的判断。但上述报错还是没有完全解决，需要进一步优化决策。


# 优化过程
目前最优达到220w，使用的权重是value/100/(机器人到源staion的距离+1.5*两个station的距离)，曾试图增加阻塞源station和目的staion的权重，但起了反效果。

- 2023.3.24

正式赛分数达到了213w，使用的权重是: *inflation\*value/100/(机器人到源staion的距离+1.5*两个station的距离)

通货膨胀系数inflation=1.0*product_num/product_info_table[d_type]/10，如果product_info_table[d_type]=0，则inflation=2

updateAvailList的准则是，对于source和dest，getTimeOfStation都要小于200

分配任务时，使用了isTaskStationRepeat认证，阈值都是2