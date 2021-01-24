#ifndef LOCKER_H
#define LOCKER_H
//�����߳�ͬ�����Ʒ�װ����
#include<exception>
#include<pthread.h>
#include<semaphore.h>

//��װ�ź�������
class sem
{
private:
	sem_t m_sem;
public:
	//��������ʼ���ź���
	sem()
	{
		if (sem_init(&m_sem, 0, 0) != 0)
			throw std::exception();
	}
	//�����ź���
	~sem()
	{
		sem_destroy(&m_sem);
	}
	//�ȴ��ź���
	bool wait()
	{
		return sem_wait(&m_sem) == 0;
	}
	//�����ź���
	bool post()
	{
		return sem_post(&m_sem) == 0;
	}
};
//��װ����������
class locker
{
public:
	//��������ʼ��������
	locker()
	{
		if (pthread_mutex_init(&m_mutex, NULL) != 0)
			throw std::exception();
	}
	//���ٻ�����
	~locker()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	//��ȡ������
	bool lock()
	{
		return pthread_mutex_lock(&m_mutex) == 0;
	}
	//�ͷŻ�����
	bool unlock()
	{
		return pthread_mutex_unlock(&m_mutex) == 0;
	}
private:
	pthread_mutex_t m_mutex;
};

#endif