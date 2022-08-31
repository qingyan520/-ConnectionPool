#include"Common.h"

class Connection
{
public:
	Connection();

	~Connection();

	bool connect(string ip, string user, string password, string dbname, int port);

	//�տ�ʼ��ȡ���ӵ�ʱ��
	void ResetConnectionTime()
	{
		this->connectionTime =clock();
	}

	//��ȡ���Ӵ���ʱ��
	int GetAliveTime()
	{
		return clock() - connectionTime;
	}
	
	//��������ɾ���Ĺ���ģ��
	bool Update(string sql);

	//�����ѯģ��
	MYSQL_RES* Query(string sql);

private:
	MYSQL* _conn;
	clock_t connectionTime=0;
};