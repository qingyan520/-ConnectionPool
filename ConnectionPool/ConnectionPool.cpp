#include"ConnectionPool.h"




//���������ļ�
bool ConnectionPool::LoadConfigFile()
{
	//��ȡ�ļ�
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
	LOG(WARNING, "û�������ļ�/�����ļ������Ϲ淶�����ֶ�������Ϣ");
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

	//д�������ļ�
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


//�������߳�
void ConnectionPool::Producer()
{
	//�����������ӣ���ҪΪ������������ģ��
	while (1)
	{
		unique_lock<mutex> _lck(this->_mtx);

		//���в�Ϊ�գ��������߳̽���ȴ�״̬�������ͷŶ�Ӧ����
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
		

		//֪ͨ�����߳̽�������
		this->_cv.notify_all();
	}
}

//�õ�ÿһ�����ӣ��൱���������߳�
shared_ptr<Connection> ConnectionPool::GetConnection()
{
	unique_lock<mutex>_lck(this->_mtx);

	//��ʱ����Ϊ�գ�û�����ӿ���ʹ�ã�����ȴ�״̬���ͷŶ�Ӧ�����������߽�������
	//�ڹ涨ʱ���ڽ��еȴ��������ʱ�ͷ��ؿ�����
	while (this->_que.empty())
	{
		//�ڹ涨ʱ���ڻ�ȡ���ӣ������ʱ�˻���
		if (cv_status::timeout == this->_cv.wait_for(_lck, std::chrono::milliseconds(this->connectionTimeOut * 1000)))
		{
			if (this->_que.empty())
			{
				LOG(WARNING, "GetConnection TimeOut!");
				continue;
			}
		}
	}

	//��ȡ����
	shared_ptr<Connection> _ptr(this->_que.front(),
		[&](Connection* ptr)->void
		{
			unique_lock<mutex>_lock(this->_mtx);
			ptr->ResetConnectionTime();
			this->_que.push(ptr);
			this->_count++;
		});

	//ɾ������Ԫ�أ����Ҷ����е�Ԫ��--
	this->_que.pop();
	this->_count--;

	//֪ͨ��������
	this->_cv.notify_all();

	return _ptr;
}


//����̣߳�������ӿ���ʱ���Ƿ����
void ConnectionPool::ScansConnection()
{
	while (1)
	{
		//ÿ��һ��ʱ����һ��
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



//ConnectionPool���캯��
//1.�����ļ��ļ���
//2.��ʼ������
//3.�������̵߳ļ���
//4.��������һ���̸߳��������ӿ���
ConnectionPool::ConnectionPool()
{
	
	if (LoadConfigFile() == false)
	{
		//���û�������ļ������ֶ��������ã�Ȼ�󱣴������ļ�
		CreateConfigFile();
	}

	//��ʼ������
	for (int i = 0; i < this->initSize; i++)
	{
		Connection* ptr = new Connection;
		ptr->connect(ip, user, password, dbname, port);
		ptr->ResetConnectionTime();
		_que.push(ptr);
		this->_count++;
	}

	//�����������߳�
	thread produce(std::bind(&ConnectionPool::Producer,this));

	thread scans(std::bind(&ConnectionPool::ScansConnection, this));

	produce.detach();
	scans.detach();
}