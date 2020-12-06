#include "Settings.h"

.section	.iram1,"ax",@progbits
.literal_position
.align	4
.global	drawScanline
.type	drawScanline, @function

drawScanline:
	entry	sp, 32	

	l32i	a11, a2, 312	# MEM[(struct VgaText *)arg_5(D)]._fontHeight, D.93550
	movi	a9, 0x148	# tmp201,
	quos	a8, a4, a11	# y, scanLine, D.93550
	slli	a10, a8, 7	# D.93552, y,
	add.n	a9, a2, a9	# D.93551, arg, tmp201
	add.n	a9, a9, a10	# fourCharacters, D.93551, D.93552
	addmi	a10, a2, 0x1c00	# tmp203, arg,
	l32i	a10, a10, 200	# MEM[(struct VgaText *)arg_5(D)].Attributes, MEM[(struct VgaText *)arg_5(D)].Attributes
	l32i	a2, a2, 320	# MEM[(struct VgaText *)arg_5(D)]._fontData, MEM[(struct VgaText *)arg_5(D)]._fontData
	slli	a8, a8, 9	# D.93554, y,
	rems	a4, a4, a11	# fontRow, scanLine, D.93550
	add.n	a8, a10, a8	# attributes, MEM[(struct VgaText *)arg_5(D)].Attributes, D.93554
	add.n	a4, a2, a4	# fontData, MEM[(struct VgaText *)arg_5(D)]._fontData, fontRow
	movi.n	a12, 0x20	# tmp250,

.LOOP:
	loop	a12, .LOOP_END
	l8ui	a10, a9, 0	# D.93550, MEM[base: fourCharacters_1, offset: 0B]
	l32i.n	a13, a8, 0	# MEM[base: attributes_2, offset: 0B], attribute
	mull	a10, a10, a11	# D.93550, D.93550, D.93550
	add.n	a10, a4, a10	# tmp212, fontData, D.93550
	l8ui	a10, a10, 0	# fontPixels, *_27
	srai	a2, a10, 4	# D.93550, fontPixels,
	addx4	a2, a2, a13	#, tmp215, D.93550, attribute
	l32i.n	a2, a2, 0	# *_34, D.93554
	extui	a10, a10, 0, 4	# D.93558, fontPixels,
	s32i.n	a2, a3, 0	# MEM[base: dest_3, offset: 0B], D.93554
	addx4	a10, a10, a13	#, tmp219, D.93558, attribute
	l32i.n	a2, a10, 0	# *_39, D.93554
	l32i.n	a13, a8, 4	# MEM[base: attributes_2, offset: 4B], attribute
	s32i.n	a2, a3, 4	# MEM[base: dest_3, offset: 4B], D.93554
	l8ui	a10, a9, 1	# D.93550, MEM[base: fourCharacters_1, offset: 1B]
	mull	a10, a10, a11	# D.93550, D.93550, D.93550
	add.n	a10, a4, a10	# tmp222, fontData, D.93550
	l8ui	a10, a10, 0	# fontPixels, *_46
	srai	a2, a10, 4	# D.93550, fontPixels,
	addx4	a2, a2, a13	#, tmp225, D.93550, attribute
	l32i.n	a2, a2, 0	# *_53, D.93554
	extui	a10, a10, 0, 4	# D.93558, fontPixels,
	s32i.n	a2, a3, 8	# MEM[base: dest_3, offset: 8B], D.93554
	addx4	a10, a10, a13	#, tmp229, D.93558, attribute
	l32i.n	a2, a10, 0	# *_58, D.93554
	l32i.n	a13, a8, 8	# MEM[base: attributes_2, offset: 8B], attribute
	s32i.n	a2, a3, 12	# MEM[base: dest_3, offset: 12B], D.93554
	l8ui	a10, a9, 2	# D.93550, MEM[base: fourCharacters_1, offset: 2B]
	mull	a10, a10, a11	# D.93550, D.93550, D.93550
	add.n	a10, a4, a10	# tmp232, fontData, D.93550
	l8ui	a10, a10, 0	# fontPixels, *_65
	srai	a2, a10, 4	# D.93550, fontPixels,
	addx4	a2, a2, a13	#, tmp235, D.93550, attribute
	l32i.n	a2, a2, 0	# *_73, D.93554
	extui	a10, a10, 0, 4	# D.93558, fontPixels,
	s32i.n	a2, a3, 16	# MEM[base: dest_3, offset: 16B], D.93554
	addx4	a10, a10, a13	#, tmp239, D.93558, attribute
	l32i.n	a2, a10, 0	# *_79, D.93554
	l32i.n	a13, a8, 12	# MEM[base: attributes_2, offset: 12B], attribute
	s32i.n	a2, a3, 20	# MEM[base: dest_3, offset: 20B], D.93554
	l8ui	a10, a9, 3	# D.93550, MEM[base: fourCharacters_1, offset: 3B]
	mull	a10, a10, a11	# D.93550, D.93550, D.93550
	add.n	a10, a4, a10	# tmp242, fontData, D.93550
	l8ui	a10, a10, 0	# fontPixels, *_86
	srai	a2, a10, 4	# D.93550, fontPixels,
	addx4	a2, a2, a13	#, tmp245, D.93550, attribute
	l32i.n	a2, a2, 0	# *_94, D.93554
	extui	a10, a10, 0, 4	# D.93558, fontPixels,
	s32i.n	a2, a3, 24	# MEM[base: dest_3, offset: 24B], D.93554
	addx4	a10, a10, a13	#, tmp249, D.93558, attribute
	l32i.n	a2, a10, 0	# *_100, D.93554
	s32i.n	a2, a3, 28	# MEM[base: dest_3, offset: 28B], D.93554

	addi	a3, a3, 32	# dest, dest,
	addi	a8, a8, 16	# attributes, attributes,
	addi.n	a9, a9, 4	# fourCharacters, fourCharacters,
.LOOP_END:

	retw.n
