#include "input.h"

#include <math.h>

#define JOYDEADZONE 32


Input::Input()
{
	state = NULL; 
	xAxis = 0.0f; 
	yAxis = 0.0f;
	lastState = 0; 
	isDown = 0;
	isUp = 0;
	ltrigDown = false; 
	rtrigDown = false; 
}

Input::~Input()
{

} 
   
void Input::update()
{
	state = (cont_state_t *)maple_dev_status(maple_enum_type(0, MAPLE_FUNC_CONTROLLER));
	
	
	isDown = state->buttons & (state->buttons ^ lastState);
	isUp = lastState & (lastState ^ state->buttons); 
	lastState = state->buttons; 
}   

float Input::lxAxis()
{
	xAxis = (abs(state->joyx) < JOYDEADZONE) ? 0 : state->joyx/128.0f; 
	return xAxis; 
}

float Input::lyAxis()
{
	yAxis = (abs(state->joyy) < JOYDEADZONE) ? 0 : state->joyy/128.0f; 
	return yAxis; 
}

float Input::rxAxis()
{
	return 0.0f;
}

float Input::ryAxis() 
{
	return 0.0f;
}
    
bool Input::button(ButtonIds button)
{
	if (BN_R1 || BN_R2) return state->rtrig > 10; 
	if (BN_L1 || BN_L2) return state->ltrig > 10;
	
	return state->buttons & button;
}

bool Input::buttonDown(ButtonIds button)
{
	if ((BN_R1 || BN_R2) && !rtrigDown)
	{		
		if (state->rtrig > 10)
		{
			rtrigDown = true; 
			return true; 
		}
	}
	
	if ((BN_L1 || BN_L2) && !ltrigDown) 
	{
		if (state->ltrig > 10)
		{
			ltrigDown = true; 
			return true; 
		}
	}
	
	return isDown & button; 
}

bool Input::buttonUp(ButtonIds button)
{
	if ((BN_R1 || BN_R2) && rtrigDown)
	{		
		if (state->rtrig < 10)
		{
			rtrigDown = false; 
			return true; 
		}
	}
	
	if ((BN_L1 || BN_L2) && ltrigDown) 
	{
		if (state->ltrig < 10)
		{
			ltrigDown = false; 
			return true; 
		}
	}
	
	return isUp & button; 
}
    
bool Input::eventQuit()
{
	return quit; 
}

Input gInput; 
