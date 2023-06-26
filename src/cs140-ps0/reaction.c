#include "pintos_thread.h"
#include <stdio.h>
// Forward declaration. This function is implemented in reaction-runner.c,
// but you needn't care what it does. Just be sure it's called when
// appropriate within reaction_o()/reaction_h().
void make_water();
// 这个实验中，reaction是全局的单例。
// 这个例子的关键是一个o原子要一次性结合2个h原子，不允许出现o原子占用一个h原子的情况。
struct reaction {
	struct condition h_cond;
	struct lock lock;
	struct condition o_cond;
	int h;
};

void
reaction_init(struct reaction *reaction)
{
	reaction->h=0;
	cond_init(&reaction->h_cond);
	cond_init(&reaction->o_cond);
	lock_init(&reaction->lock);	
}

void
reaction_h(struct reaction *reaction)
{
	lock_acquire(&reaction->lock);
	reaction->h++;
	cond_signal(&reaction->o_cond,&reaction->lock);
	cond_wait(&reaction->h_cond, &reaction->lock);
	lock_release(&reaction->lock);
	return;
}

void
reaction_o(struct reaction *reaction)
{
	lock_acquire(&reaction->lock);
	while(reaction->h<2){
		cond_wait(&reaction->o_cond, &reaction->lock);
	}
	reaction->h-=2;
	cond_signal(&reaction->h_cond, &reaction->lock);
	cond_signal(&reaction->h_cond, &reaction->lock);
	make_water();
	lock_release(&reaction->lock);
	return;

}
