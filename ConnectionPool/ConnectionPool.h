#pragma once
#include"Connection.h"

class ConnectionPool
{
public:
	//���õ���ģʽ��ָ��
	static ConnectionPool* GetInstance()
	{
		static ConnectionPool conn;
		return &conn;
	}
	ConnectionPool(const Connection& con) = delete;
	ConnectionPool& operator=(const ConnectionPool& con) = delete;

	shared_ptr <Connection>GetConnection();
private:

	//���캯��˽�л������õ���ģʽ
	ConnectionPool(); 

	bool LoadConfigFile();//���������ļ�

	void CreateConfigFile();//���û��д�����ļ������������ļ���������ֶ��������ݣ�Ȼ�����´���һ�������ļ������´�ʹ��

	void Producer();        //�������߳�

	void ScansConnection(); //����̣߳�������ӿ���ʱ���Ƿ����


	string ip;               //Ҫ�������ݿ��ip��ַ
	string user;			 //�������ݿ���û�
	string password;		 //�û�����
	int port;				 //���ݿ�˿�
	string dbname;			 //Ҫ�������ݿ������

	int initSize;            //��ʼʱ�����е�Ԫ��
	int maxSize;             //���������
	int maxIdleTime;         //������ʱ��
	int connectionTimeOut;   //���ӳ�ʱʱ��

	queue<Connection*>_que;     //�洢ÿһ������
	mutex _mtx;                 //������
	condition_variable  _cv;    //��������
	atomic_int _count=0;          //ͳ�ƶ����е�Ԫ��

};

