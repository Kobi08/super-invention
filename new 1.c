#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int size = 0, queue[5];


void enenque(int x)
{
	pthread_t th_id = pthread_self();
	pthread_mutex_lock(&mutex);//נועל

	if (size > 4)
	{

		pthread_cond_wait(&cond, &mutex);
		queue[size] = x;
		size++;
		printf("thread %ld succeeded to insert %d into BQ\n", th_id, x);
	}

	else
	{
		queue[size] = x;
		size++;
		printf("thread %ld succeeded to insert %d into BQ\n", th_id, x);
	}
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);

}
int dequeue()
{
	int number = 0;
	pthread_mutex_lock(&mutex);
	if (size == 0)
	{
		pthread_cond_wait(&cond, &mutex);
		number = queue[size - 1];
		size--;

	}
	else
	{
		number = queue[size - 1];
		size--;


	}
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);
	return number;

}


void *ThredFun(void *vargp)
{
	int ThreadCount = 0;
	pthread_t th_id = pthread_self();
	while (ThreadCount != 10)
	{
		int num = rand() % 101;
		if (num <= 50)
		{
			printf("thread  %ld is going to insert %d into BQ\n", th_id, num);

			enenque(num);
		}

		else
		{

			printf("thread %ld is going to take %d into BQ\n", th_id, num);

			int res = dequeue(num);
			printf("thread %ld succeeded to take %d into BQ\n", th_id, res);
			ThreadCount++;
		}



	}
}


int main()
{
	srand(time(NULL));
	int i;
	pthread_t tid;
	for (i = 0; i < 5; i++)
		pthread_create(&tid, NULL, ThredFun, NULL);
	pthread_exit(NULL);
	return 0;

}