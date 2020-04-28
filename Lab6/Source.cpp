#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std;

mutex noWriters;
mutex noReaders;
mutex counterAccess;

int readersCounter = 0;

void Writer()
{
	noWriters.lock();
	noReaders.lock();
	noWriters.unlock();

	cout << "Writer " << this_thread::get_id() << " start working"<<endl;
	
	//writing
	this_thread::sleep_for(chrono::milliseconds(1000));
	
	cout << "Writer " << this_thread::get_id() << " complete the job" << endl;

	noReaders.unlock();
}

void Reader()
{
	noWriters.lock();
	counterAccess.lock();
	int prev = readersCounter;
	readersCounter++;
	if (prev == 0)
		noReaders.lock();
		noWriters.unlock();

	cout << "Reader " << this_thread::get_id() << " start working" << endl;
	
	//reading
	this_thread::sleep_for(chrono::milliseconds(1000));
		
	cout << "Reader " << this_thread::get_id() << " complete the job" << endl;
			
	readersCounter--;
	int current = readersCounter;
	if (current == 0)
		noReaders.unlock();
	counterAccess.unlock();
}

int main()
{
	setlocale(0, "");

	thread t4(Writer);
	thread t1(Reader);
	thread t3(Reader);
	thread t2(Writer);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	system("pause");
}