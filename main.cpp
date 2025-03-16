#include <cstdio>
#include <kos.h>
#include <wav/sndwav.h>

#include "ajHeaders.h" 
#include "sequences.h"

#include "input.h" 

KOS_INIT_FLAGS(INIT_DEFAULT);

int main(int argc, char* argv[])
{
	draw_init(); 

	printf("--- INTRO INIT ---\n");
	IntroInit();
	
	printf("--- NEGEFFECT INIT ---\n");	
	NegEffectInit(); 
	
	printf("--- WOBBLE CUBE INIT ---\n");	
	WobbleCubeInit();
	
	printf("--- GLASS INIT ---\n");
	GlassInit();
	
	printf("--- PLASMA INIT ---\n");
	PlasmaInit();
	
	printf("--- PLASMA 2 INIT ---\n");
	Plasma2Init();
	
	printf("--- TUNNEL INIT ---\n");
	TunnelInit();
	
	printf("--- HEX WAVE INIT ---\n");
	HexWaveInit();
	
	printf("--- TRAIL INIT ---\n");
	TrailInit();
    
    snd_stream_init();
    wav_init();
    wav_stream_hnd_t musicwav = wav_create("/rd/music.wav", 1);
    wav_volume(musicwav, 255);
    wav_play(musicwav);
    
    while(wav_is_playing==0);//don't understand this but it fixed a bug XD

	int q = 1; 

	uint64_t tickit = 2487*10 + timer_ms_gettime64();//27132
	uint64_t tick1t = 3590*10 + timer_ms_gettime64();
	uint64_t tick2t = 4694*10 + timer_ms_gettime64();

	uint64_t tick3t = 6900*10 + timer_ms_gettime64();
	uint64_t tick4t = 8000*10 + timer_ms_gettime64();
	uint64_t tick5t = 9107*10 + timer_ms_gettime64();
	
	uint64_t tick6t = 10210*10 + timer_ms_gettime64();
	uint64_t tick7t = 11170*10 + timer_ms_gettime64();
	uint64_t tick8t = 13340*10 + timer_ms_gettime64();

	while(q)
	{
		gInput.update();
		if(gInput.buttonDown(BN_TRIANGLE))q = 0;
       
       	if (tickit > timer_ms_gettime64())	
       	{
       		IntroRun();
		}
       	else if (tick1t > timer_ms_gettime64())
       	{
       		NegEffectRun();
       	}
       	else if (tick2t > timer_ms_gettime64())
       	{
       		GlassRun(); 
       	}
       	else if (tick3t > timer_ms_gettime64())
       	{
       		TunnelRun();
       	}
       	else if (tick4t > timer_ms_gettime64())
       	{
       		PlasmaRun();
       	}
       	else if (tick5t > timer_ms_gettime64())
       	{
       		Plasma2Run();
       	}
       	else if (tick6t > timer_ms_gettime64())
       	{
       		WobbleCubeRun();
       	}
       	else if (tick7t > timer_ms_gettime64())
       	{
       		HexWaveRun();
       	}
       	else if (tick8t > timer_ms_gettime64())
       	{
       		TrailRun();
       	}
       	else 
       	{
       		q = 0;	
		}	 
	}
	
	draw_close(); 
	return 0;
}
