#include"ConnectionPool.h"
int main()
{
	//Connection con;
	//con.connect("127.0.0.1", "root", "123456", "t", 3306);
	//MYSQL_RES* ret = con.Query("select * from t11");
	//if (ret)
	//{
	//	MYSQL_ROW  row = nullptr;
	//	while (row = mysql_fetch_row(ret))
	//	{
	//		cout << row[0] << " " << row[1] << endl;
	//	}
	//	mysql_free_result(ret);
	//}

	ConnectionPool::GetInstance();

	return 0;
}