/*
    Copyright 2005-2010 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.
*/

/*
    The original source for this example is
    Copyright (c) 1994-2008 John E. Stone
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. The name of the author may not be used to endorse or promote products
       derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
    OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/

#include "machine.h"
#include "types.h"
#include "macros.h"
#include "vector.h"
#include "tgafile.h"
#include "trace.h"
#include "light.h"
#include "shade.h"
#include "camera.h"
#include "util.h"
#include "intersect.h"
#include "global.h"
#include "ui.h"
#include "video.h"

// shared but read-only so could be private too
static thr_parms *all_parms;
static scenedef scene;
static int startx;
static int stopx;
static int starty;
static int stopy;
static flt jitterscale;
static int totaly;

void initialize_2D_buffer (unsigned int mem_array [], unsigned int *fill_value)
{

/***********************************************************************
// Try sets of columns method to defeat hardware prefetching:
//  Do all rows of column 0 of array then skip "col_interval" columns and do the next column
//  When that first set of columns is done, do all the rows of column 1, skip col_set columns
//     and do the next column
//  Continue until all columns are done
	int col_max = mem_array_i_max;
	int row_max = mem_array_j_max;

int iteration_count(0);

	for (int col_set = 0; col_set < col_interval; col_set++)
	{
		for (int relative_col = 0; relative_col < col_max; relative_col += col_interval)
		{
			int column = relative_col + col_set;
			for (int row = 0; row < row_max; row++)
			{
				mem_array [row*col_max+column] = *fill_value + 2;

//iteration_count++;
//if (iteration_count < 50)
//   printf (" iteration_count = %d  array_index = %d \n", iteration_count, (row*col_max+column));

			}
		}
	}
/******************************************************/

/*****************************************
        // First (slower) method of filling array
        // Array is NOT filled in consecutive memory address order
        /**********************************/
        for (int i = 0; i < mem_array_i_max; i++)
        {
/************************/
// Try to defeat hardware prefetching by varying the stride
int j(0), iteration_count(0);

do {
   mem_array [j*mem_array_i_max+i] = *fill_value + 2;

   // Code to give the array accesses a non-uniform stride to defeat hardware prefetch
   if ((iteration_count % 3) == 0) j=j+3;
   else j=j+2;
   iteration_count++;
} while (j < mem_array_j_max);
        }
/************************
/************************************************

int iteration_count(0);
               for (int j = 0; j < mem_array_j_max; j++)
               {
                        mem_array [j*mem_array_i_max+i] = *fill_value + 2;
//iteration_count++;
//if (iteration_count < 50)
//   printf (" iteration count = %d    array index = %d \n", iteration_count, (j*mem_array_i_max+i));
               }
       }
        /***********************************/


        // Faster method of filling array
        // The for loops are interchanged
        // Array IS filled in consecutive memory address order
        /**********************************
        for (int j = 0; j < mem_array_j_max; j++)
        {
                for (int i = 0; i < mem_array_i_max; i++)
                {
                        mem_array [j*mem_array_i_max+i] = *fill_value + 2;
                }
        }
        /***********************************/
}

