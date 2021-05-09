#pragma once

#pragma warning(suppress : 4996)

#include <iostream>
#include <assert.h>
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef unsigned char		byte;
typedef unsigned char		u8;
typedef signed char			s8;
typedef unsigned short		u16;
typedef unsigned long		u32;
typedef signed long			s32;
typedef unsigned __int64	u64;
typedef double				f64;


#define MASK_ALLPLAYERS_3     0x7		
#define MASK_ALLPLAYERS_4	  0xF				
#define MASK_ALLPLAYERS_5	 0x1F			
#define MASK_ALLPLAYERS_6	 0x3F			
#define MASK_ALLPLAYERS_7	 0x7F			
#define MASK_ALLPLAYERS_8	 0xFF				
#define MASK_ALLPLAYERS_9	0x1FF			
#define MASK_ALLPLAYERS_10	0x3FF				

// ----------------------------------------------------------------------------------------------
#define PLAYERNB							6	// nombre total de joueurs
#define MASK_ALLPLAYERS		MASK_ALLPLAYERS_6	// tous les bits de 0 à (PLAYERNB-1) à 1

#define PLAYER_PERMUT0			2,3,5,4			// pour 6
#define TARGETS_LIST		1,2,0,4,5,3			// pour 6

//#define PLAYER_PERMUT0			2,3,5,4,6			// pour 7
//#define TARGETS_LIST		1,2,0,4,5,6,3		// pour 7

//#define PLAYER_PERMUT0	  3,7,4,5,6,2		// pour 8		
//#define TARGETS_LIST		1,2,0,4,5,6,7,3		// pour 8

//#define PLAYER_PERMUT0		  3,7,4,5,6,8,2			// pour 9		
//#define TARGETS_LIST		1,2,0,4,5,6,7,8,3		// pour 9
// -----------------------------------------------------------------------------------------------

// Permutation de byte dans un tableau de byte de taille sz
void permute(byte** ppr, byte* a, const byte l, const byte sz);

// Permutation de nibble dans un 32 bits de taille sz ( max = 32/4 = 8 )
u32* permuteu32(u32* pr, u32 *pa, const byte l, const byte sz);
