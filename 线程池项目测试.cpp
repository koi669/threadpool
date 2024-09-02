// �̳߳���Ŀ.cpp : ���ļ����� "main" ����������ִ�н��ڴ˴���ʼ��������
//

#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

#include "threadpool.h"

/*
��Щ��������ϣ���ܹ���ȡ�߳�ִ������÷���ֵ��
������
1 + ������ + 30000�ĺ�
thread1  1 + ... + 10000
thread2  10001 + ... + 20000
.....

main thread����ÿһ���̷߳����������䣬���ȴ��������귵�ؽ�����ϲ����յĽ������
*/

using uLong = unsigned long long;

class MyTask : public Task
{
public:
    MyTask(int begin, int end)
        : begin_(begin)
        , end_(end)
    {}
    // ����һ����ô���run�����ķ���ֵ�����Ա�ʾ���������
    // Java Python   Object ���������������͵Ļ���
    // C++17 Any����
    Any run()  // run�������վ����̳߳ط�����߳���ȥ��ִ����!
    {
        std::cout << "tid:" << std::this_thread::get_id()
            << "begin!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3));
        uLong sum = 0;
        for (uLong i = begin_; i <= end_; i++)
            sum += i;
        std::cout << "tid:" << std::this_thread::get_id()
            << "end!" << std::endl;

        return sum;
    }

private:
    int begin_;
    int end_;
};

int main()
{
    {
        ThreadPool pool;
        pool.setMode(PoolMode::MODE_CACHED);
        // ��ʼ�����̳߳�
        pool.start(2);

        // linux�ϣ���ЩResult����Ҳ�Ǿֲ�����Ҫ�����ģ�����
        Result res1 = pool.submitTask(std::make_shared<MyTask>(1, 100000000));
        Result res2 = pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
        pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
        pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
        pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));

        //uLong sum1 = res1.get().cast_<uLong>();
        //cout << sum1 << endl; 
    } // ����Result����ҲҪ����!!! ��vs�£����������������ͷ���Ӧ��Դ��

    cout << "main over!" << endl;
    getchar();
#if 0
    // ���⣺ThreadPool���������Ժ���ô�����̳߳���ص��߳���Դȫ�����գ�
    {
        ThreadPool pool;
        // �û��Լ������̳߳صĹ���ģʽ
        pool.setMode(PoolMode::MODE_CACHED);
        // ��ʼ�����̳߳�
        pool.start(4);

        // �����������Result������
        Result res1 = pool.submitTask(std::make_shared<MyTask>(1, 100000000));
        Result res2 = pool.submitTask(std::make_shared<MyTask>(100000001, 200000000));
        Result res3 = pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));
        pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));

        pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));
        pool.submitTask(std::make_shared<MyTask>(200000001, 300000000));

        // ����task��ִ���꣬task����û�ˣ�������task�����Result����Ҳû��
        uLong sum1 = res1.get().cast_<uLong>();  // get������һ��Any���ͣ���ôת�ɾ���������أ�
        uLong sum2 = res2.get().cast_<uLong>();
        uLong sum3 = res3.get().cast_<uLong>();

        // Master - Slave�߳�ģ��
        // Master�߳������ֽ�����Ȼ�������Slave�̷߳�������
        // �ȴ�����Slave�߳�ִ�������񣬷��ؽ��
        // Master�̺߳ϲ����������������
        cout << (sum1 + sum2 + sum3) << endl;
    }



    /*uLong sum = 0;
    for (uLong i = 1; i <= 300000000; i++)
        sum += i;
    cout << sum << endl;*/

    /*pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());
    pool.submitTask(std::make_shared<MyTask>());*/

    getchar();

#endif
}