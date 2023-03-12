#ifndef decision_h
#define decision_h

//ѡ�����͵�����
const int DECISION_TYPE_NUM = 5;



//ѡ����Ϣ
typedef struct {
    int type;       //0-4��ѡ������
    int robot;      //������ID
    float param;    //����2
} DecisionInfo;

//ѡ�������
extern int decision_num;


//ѡ�����Ͷ�Ӧ���ַ���
extern char decision_string[DECISION_TYPE_NUM][10];
//ѡ����Ϣ��
extern DecisionInfo decision_info_table[50];

//����ѡ��
void makeDecision();
//���ѡ��
void outputDecision();






#endif