#include "pintos_thread.h"
#include <stdio.h>

struct station {
	int passenger_num;
	struct condition a_cond;	
	struct lock lock; 
	struct condition b_cond;
};

void
station_init(struct station *s)
{
	cond_init(&s->a_cond);
	lock_init(&s->lock);	
	cond_init(&s->b_cond);
	s->passenger_num=0;
}

void
station_load_train(struct station *station, int count)
{
    lock_acquire(&station->lock);
	while(1){
		if(station->passenger_num>0&&count>0){
			cond_signal(&station->a_cond,&station->lock);
			cond_wait(&station->b_cond,&station->lock);
		}else{
			lock_release(&station->lock);
			return;
		}
		count--;
	}
}

void
station_wait_for_train(struct station *station)
{
	lock_acquire(&station->lock);
	station->passenger_num+=1;
	cond_wait(&station->a_cond, &station->lock);
	lock_release(&station->lock);
	return;
}

void
station_on_board(struct station *station)
{
	lock_acquire(&station->lock);
	station->passenger_num-=1;
	cond_signal(&station->b_cond,&station->lock);
	lock_release(&station->lock);
	return;
}
