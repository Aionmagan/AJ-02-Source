#ifndef __INPUT_H__
#define __INPUT_H__

#include <dc/maple.h>
#include <dc/maple/controller.h>


enum ButtonIds
{
	BN_LEFT     = (1<<6),
	BN_DOWN     = (1<<5),
	BN_RIGHT    = (1<<7),
	BN_UP       = (1<<4),
	BN_START    = (1<<3),
	BN_R3       = 0xff,
	BN_L3       = 0xff,
	BN_SELECT   = 0xff,
	BN_SQUARE   = (1<<10),
	BN_CROSS    = (1<<2),
	BN_CIRCLE   = (1<<1),
	BN_TRIANGLE = (1<<9),
	BN_R1       = 0xdf,
	BN_L1       = 0xde,
	BN_R2       = 0xdf,
	BN_L2       = 0xde
};

class Input
{
public: 
	Input(); 
   ~Input(); 
	
	void  setPort(int p);    
    void  update(); 
    
    float lxAxis(); 
    float lyAxis(); 
    float rxAxis(); 
    float ryAxis(); 
    
    bool button(ButtonIds button); 
    bool buttonDown(ButtonIds button);
    bool buttonUp(ButtonIds button); //implement later
    
    bool eventQuit(); //should be it's own thing
    
    void Init(); 
    
private: 
	bool  quit; 
	int   isDown;
	int   isUp;
	
	cont_state_t* state; 
	float xAxis; 
	float yAxis; 
	int lastState; 
	bool ltrigDown; 
	bool rtrigDown; 
};

extern Input gInput; 

#endif/*__INPUT_H__*/
