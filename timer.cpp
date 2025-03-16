#include "timer.h"
#include <stdlib.h>
#include <arch/timer.h>

#define LOW_LIMIT 0.0167
#define HIGH_LIMIT 0.1

Time::Time()
{
	srand(time(NULL)); 
	old_time = getTicks(); 
}
 
Time::~Time()
{

}

float Time::deltaTime()
{
	return delta_time;///1000.0f; 
}

float Time::frameRate()
{
	return (1.0 / delta_time) * 1000.0f;
}

unsigned long Time::getTicks()
{
	return (unsigned long) timer_ms_gettime64(); 
}

void Time::update()
{
	new_time = getTicks(); 
	delta_time = (new_time - old_time) * 0.001f; 
	old_time = new_time; 	
	if (deltaTime() < LOW_LIMIT) 
		delta_time = LOW_LIMIT; 
	else if (deltaTime() > HIGH_LIMIT)
		delta_time = HIGH_LIMIT; 
}

Time gTime; 
