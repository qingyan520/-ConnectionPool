#include"ConnectionPool.h"




//加载配置文件
bool ConnectionPool::LoadConfigFile()
{
	//读取文件
	ifstream fin;
	fin.open("my.ini", ios::in);
	if (!fin.is_open())
	{
		LOG(FALTA, "Not Find ConfigFile my.ini!");
		return false;
	}

	char buf[10240] = { 0 };
	while (fin.getline(buf, sizeof(buf)))
	{
		string key;
		string val;
		if (CutString(buf, key, val) == false)
		{
			continue;
		}
		//cout << key << ":" << val << endl;
		if (key == "ip")
		{
			this->ip = val;
			//cout << "IP:" << this->ip << endl;
		}
		else if (key == "user")
		{
			this->user = val;
			//cout << "user:" << this->user << endl;
		}
		else if (key == "password")
		{
			this->password = val;
			//cout << "password:" << this->password << endl;
		}
		else if (key == "dbname")
		{
			this->dbname = val;
			//cout << "dbname:" << this->dbname << endl;
		}
		else if (key == "port")
		{
			this->port = std::atoi(val.c_str());
			//cout << "port:" << this->port << endl;
		}
		else if (key == "initSize")
		{
			this->initSize = std::atoi(val.c_str());
		}
		else if (key == "maxSize")
		{
			this->maxSize = std::atoi(val.c_str());
		
		}
		else if (key == "maxIdleTime")
		{
			this->maxIdleTime = std::atoi(val.c_str());
		}
		else if (key == "connectionTimeOut")
		{
			this->connectionTimeOut = std::atoi(val.c_str());
		}
		else
		{
			LOG(FALTA, "LoadConfigFlie error!");
			return false;
		}
	}
	fin.close();
	return true;
}


void ConnectionPool::CreateConfigFile()
{
	LOG(WARNING, "没有配置文件/配置文件不符合规范，请手动输入信息");
	cout << "ip:";
	cin >> ip;
	cout << "user:";
	cin >> user;
	cout << "password:";
	cin >> password;
	cout << "dbname:";
	cin >> dbname;
	cout << "port:";
	cin >> port;
	cout << "initSize:";
	cin >> initSize;
	cout << "maxSize:";
	cin >> maxSize;
	cout << "maxIdleTime:";
	cin >> maxIdleTime;
	cout << "connectionTimeOut:";
	cin >> connectionTimeOut;

	//写入配置文件
	ofstream fout;
	fout.open("my.ini", ios::out);
	if (!fout.is_open())
	{
		LOG(WARNING, "Create ConfigFile error!");
	}
	fout << "ip=" << ip << endl;
	fout << "user=" << user << endl;
	fout << "password=" << password << endl;
	fout << "dbname=" << dbname << endl;
	fout << "port=" << port << endl;
	fout << "initSize=" << initSize << endl;
	fout << "maxSize=" << maxSize << endl;
	fout << "maxIdleTime=" << maxIdleTime << endl;
	fout << "connectionTimeOut=" << connectionTimeOut << endl;
}


//生产者线程
void ConnectionPool::Producer()
{
	//负责生产连接，主要为生产者消费者模型
	while (1)
	{
		unique_lock<mutex> _lck(this->_mtx);

		//队列不为空，生产者线程进入等待状态，并且释放对应的锁
		while (_count>=initSize/2)
		{
			_cv.wait(_lck);
		}

		
	//	if (this->_count < this->maxSize)
		
			Connection* ptr = new Connection;
			ptr->connect(ip, user, password, dbname, port);
			ptr->ResetConnectionTime();
			_que.push(ptr);
			this->_count++;
		

		//通知其它线程进行消费
		this->_cv.notify_all();
	}
}

//得到每一个连接，相当于消费者线程
shared_ptr<Connection> ConnectionPool::GetConnection()
{
	unique_lock<mutex>_lck(this->_mtx);

	//此时队列为空，没有连接可以使用，进入等待状态，释放对应的锁让生产者进行生产
	//在规定时间内进行等待，如果超时就返回空连接
	while (this->_que.empty())
	{
		//在规定时间内获取连接，如果超时了或者
		if (cv_status::timeout == this->_cv.wait_for(_lck, std::chrono::milliseconds(this->connectionTimeOut * 1000)))
		{
			if (this->_que.empty())
			{
				LOG(WARNING, "GetConnection TimeOut!");
				continue;
			}
		}
	}

	//获取连接
	shared_ptr<Connection> _ptr(this->_que.front(),
		[&](Connection* ptr)->void
		{
			unique_lock<mutex>_lock(this->_mtx);
			ptr->ResetConnectionTime();
			this->_que.push(ptr);
			this->_count++;
		});

	//删除队首元素，并且队列中的元素--
	this->_que.pop();
	this->_count--;

	//通知其它连接
	this->_cv.notify_all();

	return _ptr;
}


//监控线程，监控连接空闲时间是否过长
void ConnectionPool::ScansConnection()
{
	while (1)
	{
		//每隔一段时间检测一次
	//	std::this_thread::sleep_for(std::chrono::microseconds(this->maxIdleTime * 1000));
		while (this->_count > this->initSize)
		{
			Connection* ptr = this->_que.front();
			if (ptr->GetAliveTime() > maxIdleTime * 1000)
			{
				_que.pop();
				delete ptr;
			}
			else
			{
				break;
			}
		}
	}
}



//ConnectionPool构造函数
//1.配置文件的加载
//2.初始化队列
//3.生产者线程的加载
//4.另外再起一个线程负责监控连接空闲
ConnectionPool::ConnectionPool()
{
	
	if (LoadConfigFile() == false)
	{
		//如果没有配置文件，就手动输入配置，然后保存配置文件
		CreateConfigFile();
	}

	//初始化队列
	for (int i = 0; i < this->initSize; i++)
	{
		Connection* ptr = new Connection;
		ptr->connect(ip, user, password, dbname, port);
		ptr->ResetConnectionTime();
		_que.push(ptr);
		this->_count++;
	}

	//开启生产者线程
	thread produce(std::bind(&ConnectionPool::Producer,this));

	thread scans(std::bind(&ConnectionPool::ScansConnection, this));

	produce.detach();
	scans.detach();
}