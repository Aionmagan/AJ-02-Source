#ifndef __AE_TIMER_H__
#define __AE_TIMER_H__


#include <kos.h>


class Time
{
public: 
	Time(); 
   ~Time(); 
	float         deltaTime();
	float         frameRate();  
	unsigned long getTicks(); 
	void          update(); 
	
private:
	float delta_time; 
 	unsigned int new_time, old_time; 
	unsigned int ticks;
};

extern Time gTime; 

#endif/*__TIMER_H__*/
