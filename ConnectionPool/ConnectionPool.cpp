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

//ConnectionPool构造函数
//1.配置文件的加载
//2.初始化队列
//3.生产者线程的加载
//4.另外再起一个线程负责监控连接空闲
ConnectionPool::ConnectionPool()
{
	
	if (LoadConfigFile() == false)
	{
		CreateConfigFile();
	}


}