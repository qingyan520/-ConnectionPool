#include"Common.h"

class Connection
{
public:
	Connection();

	~Connection();

	bool connect(string ip, string user, string password, string dbname, int port);
	
	//��������ɾ���Ĺ���ģ��
	bool Update(string sql);

	//�����ѯģ��
	MYSQL_RES* Query(string sql);

private:
	MYSQL* _conn;
};