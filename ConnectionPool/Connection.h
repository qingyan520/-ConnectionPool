#include"Common.h"

class Connection
{
public:
	Connection();

	~Connection();

	bool connect(string ip, string user, string password, string dbname, int port);
	
	//负责增，删，改功能模块
	bool Update(string sql);

	//负责查询模块
	MYSQL_RES* Query(string sql);

private:
	MYSQL* _conn;
};