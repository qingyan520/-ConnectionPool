#pragma once
#include"Connection.h"

class ConnectionPool
{
public:
	//设置单例模式的指针
	static ConnectionPool* GetInstance()
	{
		static ConnectionPool conn;
		return &conn;
	}
	ConnectionPool(const Connection& con) = delete;
	ConnectionPool& operator=(const ConnectionPool& con) = delete;

	shared_ptr <Connection>GetConnection();
private:

	//构造函数私有化，设置单例模式
	ConnectionPool(); 

	bool LoadConfigFile();//加载配置文件

	void CreateConfigFile();//如果没有写配置文件，或者配置文件有问题就手动输入数据，然后重新创建一个配置文件方便下次使用

	void Producer();        //生产者线程

	void ScansConnection(); //监控线程，监控连接空闲时间是否过长


	string ip;               //要访问数据库的ip地址
	string user;			 //访问数据库的用户
	string password;		 //用户密码
	int port;				 //数据库端口
	string dbname;			 //要访问数据库的名称

	int initSize;            //初始时队列中的元素
	int maxSize;             //最大连接量
	int maxIdleTime;         //最大空闲时间
	int connectionTimeOut;   //连接超时时间

	queue<Connection*>_que;     //存储每一个连接
	mutex _mtx;                 //互斥锁
	condition_variable  _cv;    //条件变量
	atomic_int _count=0;          //统计队列中的元素

};

