#include"ConnectionPool.h"



void Sql()
{
	int n = 10;
	char sql[] = "insert into t1(name,age) values('张三',20)";

	clock_t begin = clock();
	for (int i = 0; i < n; i++)
	{
		auto ptr = ConnectionPool::GetInstance()->GetConnection();
		if(ptr!=nullptr)
		ptr->Update(sql);
	}
	clock_t end = clock();
	//cout <<std::this_thread::get_id() << "使用连接池:" << end - begin << "ms" << endl;

	printf("%lu ms\n", end - begin);



	//Connection my;
	//my.connect("127.0.0.1", "root", "123456", "t", 3306);
	//clock_t begin=clock();
	//for (int i = 0; i < n; i++)
	//{
	//
	//	my.Update(sql);
	//}

	//clock_t end = clock();
	//cout << "未使用连接池:" << end - begin << "ms" << endl;
}
int main()
{
	auto ptr = ConnectionPool::GetInstance();
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

	//Connection my;
	//my.connect("127.0.0.1", "root", "123456", "t", 3306);
	//clock_t begin=clock();
	//for (int i = 0; i < n; i++)
	//{
	//
	//	my.Update(sql);
	//}

	//clock_t end = clock();
	//cout << "未使用连接池:" << end - begin << "ms" << endl;


	//clock_t begin = clock();
	//for (int i = 0; i < n; i++)
	//{
	//	auto ptr = ConnectionPool::GetInstance()->GetConnection();
	//	ptr->Update(sql);
	//}
	//clock_t end = clock();
	//cout << "使用连接池:" << end - begin << "ms"<<endl;

	thread *th=new thread[20000];

	for (int i = 0; i < 20000; i++)
	{
		th[i] = thread(Sql);
	}


	for (int i = 0; i < 20000; i++)
	{
		th[i].join();
	}

	/*thread t1(Sql);
	thread t2(Sql);
	t2.join();
	t1.join();*/

	return 0;
}