#ifndef checker_io_h
#define checker_io_h

using namespace std;
const int DOT_NUM = 100;


//����һ��ok
void sendOK();
//��ȡ�����ͼ
bool readMap();
//������ĵ�ǰ֡��ͷ��
bool readFrameHead();
//��ȡ����ĵ�ǰ֡��ʣ��������Ϣ
void readFrameAll();
//���ѡ����Ϣ
void writeDecision();
//�趨ddl
void setDDL(int ms);
//���˯��ddl
void waitDDL();







#endif