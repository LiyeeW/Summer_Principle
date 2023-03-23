#ifndef move_conf_all_h
#define move_conf_all_h





//冲突方案的基础函数的共同签名，便于使用函数指针数组
typedef void(*ConFun)(int a,int b);

//识别任何可能存在的冲突，将识别结果
void recognizeAnyConf();

#endif