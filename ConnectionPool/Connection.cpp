#include"Connection.h"

//���캯��
Connection::Connection()
{
	_conn = mysql_init(nullptr);
}

//��������
Connection::~Connection()
{
	if (_conn != nullptr)
	{
		mysql_close(_conn);
	}
}


//����ָ�����ݿ�
bool Connection::connect(string ip, string user, string password, string dbname, int port)
{
	mysql_real_connect(_conn, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
	if (_conn == nullptr)
	{
		LOG(WARNING, "mysql_real_connect error!");
		return false;
	}
	return true;
}

//�������ݵ���ɾ��
bool Connection::Update(string sql)
{
	mysql_query(_conn, "set names gbk");
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG(WARNING, mysql_error(_conn));
		return false;
	}
	return true;
}

MYSQL_RES* Connection::Query(string sql)
{
	mysql_query(_conn, "set names gbk");
	if (mysql_query(_conn, sql.c_str()))
	{
		sql += ",";
		LOG(WARNING, sql+mysql_error(_conn));
		return nullptr;
	}
	return mysql_use_result(_conn);
}