#/********************************************************************************************/
#/* KallistiOS Direct PVR API:2.0  PVR TEXTURE CUBE WITH ZOOM AND ROTATION  V1               */
#/********************************************************************************************/
#/********************************************************************************************/
#/* KallistiOS Direct PVR API:2.0  PVR TEXTURE CUBE WITH 6 TEXTURES: ZOOM AND ROTATION, V2.0 */
#/********************************************************************************************/
#/********************************************************************************************/
#/* Name:     cube6.c                                                                        */
#/* Title:    PVR TEXTURE CUBE WITH 6 TEXTURES: ZOOM AND ROTATION, V2.0 Kos Example          */
#/* Author:   (c)Ian Micheal                                                                 */
#/* Created:  08/05/24                                                                       */
#/*                                                                                          */
#/* Version:  2.0                                                                            */
#/* Platform: Dreamcast | KallistiOS:2.0 | KOSPVR |                                          */
#/*                                                                                          */
#/* Description: 6 Textures one each cube face                                               */
#/* The purpose of this example is to show the use of only the KOSPVR API to do 3D           */
#/* And commented so anyone who knows OpenGL can use the DIRECT NO LAYER KOSPVR API.         */
#/* History: version 2                                                                       */
#/********************************************************************************************/
#/********************************************************************************************/
#/*        >>>  Help and code examples and advice these people where invaluable  <<<         */
#/*     Mvp's:  dRxL with my_perspective_mat_lh and explaining to me the concepts            */
#/*     Mvp's:  Bruce tested and found both annoying bugs and texture distortion.            */
#/*                                                                                          */
#/********************************************************************************************/ 
KOS_CFLAGS+= -g 
TARGET = aj02.elf

#OBJS = tunnel.o rubix.o display.o ply.o
SRC = $(wildcard *.cpp)
OBJS = romdisk.o $(notdir $(SRC:%.cpp=%.o))

KOS_ROMDISK_DIR = romdisk

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	-rm -f $(TARGET) $(OBJS) romdisk.*
rm-elf:
	-rm -f $(TARGET) romdisk.*

$(TARGET): $(OBJS) romdisk.o
	kos-c++ -o $(TARGET) $(OBJS) $(DATAOBJS) $(OBJEXTRA) -lwav -lpng -ljpeg -lkmg -lz -lkosutils -lm -ffast-math
	
sim: $(TARGET)
	flatpak run org.flycast.Flycast $(TARGET)
	
lx: $(TARGET)
	lxdream $(TARGET)
	
run: $(TARGET)
	echo qwer | sudo -kS /opt/toolchains/dc/bin/dc-tool-ser -t /dev/ttyUSB0 -b 1500000 -x $(TARGET)

dist:
	rm -f $(OBJS) romdisk.o romdisk.img
	$(KOS_STRIP) $(TARGET)
