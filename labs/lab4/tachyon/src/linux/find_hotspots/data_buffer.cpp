//==============================================================
//
// SAMPLE SOURCE CODE - SUBJECT TO THE TERMS OF SAMPLE CODE LICENSE AGREEMENT,
// http://software.intel.com/en-us/articles/intel-sample-source-code-license-agreement/
//
// Copyright 2010 Intel Corporation
//
// THIS FILE IS PROVIDED "AS IS" WITH NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
// NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, NON-INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS.
//
// ===============================================================

#include "types.h"
#include "find_hotspots.h"

// buffer used in find_hotspots.cpp for Loop Interchange optimization demonstration
unsigned int mem_array [mem_array_i_max * mem_array_j_max];
unsigned int fill_value = 4;

void setup_2D_buffer ()
{
	// initialize_2D_buffer is where the loop interchange occurs
	initialize_2D_buffer (mem_array, &fill_value);
}

