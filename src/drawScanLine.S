#include "Settings.h"

.section	.iram1,"ax",@progbits
.literal_position
.align	4
.global	drawScanline
.type	drawScanline, @function

# Arguments:
#  a2  VgaText* controller
#  a3  uint8_t* dest
#  a4  int      scanLine
drawScanline:
	entry	sp, 32	

	l32i	a11, a2, 312    # a11 = fontHeight
	movi	a9, 0x148	
	quos	a8, a4, a11	
	slli	a10, a8, 7	
	add.n	a9, a2, a9	
	add.n	a9, a9, a10	    # a9 = fourCharacters
	addmi	a10, a2, 0x1c00
	l32i	a10, a10, 200
	l32i	a2, a2, 320	
	slli	a8, a8, 9	
	rems	a4, a4, a11
	add.n	a8, a10, a8	    # a8 = attributes
	add.n	a4, a2, a4	    # a4 = fontData

	movi.n	a12, (SCREEN_WIDTH / 4)
.LOOP:
	loop	a12, .LOOP_END

	l32i.n	a13, a8, 0	    #  a13 = attributes[0]

	l8ui	a10, a9, 0	    #  a10 = fourCharacters[0]
	mull	a10, a10, a11	#  
	add.n	a10, a4, a10	#  
	l8ui	a10, a10, 0	    #  

	srai	a2, a10, 4	    #  a2 = a10 >> 4
	addx4	a2, a2, a13	    #  
	l32i.n	a2, a2, 0	    #  
	s32i.n	a2, a3, 0	    #  dest32[0] = a2

	extui	a10, a10, 0, 4	#  a10 = a10 & 0x0F
	addx4	a10, a10, a13	# 
	l32i.n	a2, a10, 0	    # 
	s32i.n	a2, a3, 4	    #  dest32[4] = a2

	l32i.n	a13, a8, 4	   
	l8ui	a10, a9, 1	   
	mull	a10, a10, a11	
	add.n	a10, a4, a10	
	l8ui	a10, a10, 0	    
	srai	a2, a10, 4	    
	addx4	a2, a2, a13	    
	l32i.n	a2, a2, 0	    
	extui	a10, a10, 0, 4	
	s32i.n	a2, a3, 8	    
	addx4	a10, a10, a13	
	l32i.n	a2, a10, 0	    
	s32i.n	a2, a3, 12	    

	l32i.n	a13, a8, 8	    
	l8ui	a10, a9, 2
	mull	a10, a10, a11
	add.n	a10, a4, a10
	l8ui	a10, a10, 0
	srai	a2, a10, 4
	addx4	a2, a2, a13
	l32i.n	a2, a2, 0
	extui	a10, a10, 0, 4
	s32i.n	a2, a3, 16
	addx4	a10, a10, a13
	l32i.n	a2, a10, 0
	s32i.n	a2, a3, 20

	l32i.n	a13, a8, 12
	l8ui	a10, a9, 3
	mull	a10, a10, a11
	add.n	a10, a4, a10
	l8ui	a10, a10, 0
	srai	a2, a10, 4
	addx4	a2, a2, a13
	l32i.n	a2, a2, 0
	extui	a10, a10, 0, 4
	s32i.n	a2, a3, 24
	addx4	a10, a10, a13
	l32i.n	a2, a10, 0
	s32i.n	a2, a3, 28

	addi	a3, a3, 32	# dest += 32
	addi	a8, a8, 16	# attributes += 16
	addi.n	a9, a9, 4	# fourCharacters += 4
.LOOP_END:

	retw.n
