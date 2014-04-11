/*
    Copyright 2005-2009 Intel Corporation.  All Rights Reserved.

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

/* 
 * sphere.c - This file contains the functions for dealing with spheres.
 *
 *  $Id: sphere.cpp,v 1.2 2007-02-22 17:54:16 dpoulsen Exp $
 */
 
#include "machine.h"
#include "types.h"
#include "macros.h"
#include "vector.h"
#include "intersect.h"
#include "util.h"

#define SPHERE_PRIVATE
#include "sphere.h"

static object_methods sphere_methods = {
  (void (*)(void *, void *))(sphere_intersect),
  (void (*)(void *, void *, void *, void *))(sphere_normal),
  sphere_bbox, 
  free 
};

object * newsphere(void * tex, vector ctr, flt rad) {
  sphere * s;
  
  s=(sphere *) rt_getmem(sizeof(sphere));
  memset(s, 0, sizeof(sphere));
  s->methods = &sphere_methods;

  s->tex=(texture *)tex;
  s->ctr=ctr;
  s->rad=rad;

  return (object *) s;
}

static int sphere_bbox(void * obj, vector * min, vector * max) {
  sphere * s = (sphere *) obj;

  min->x = s->ctr.x - s->rad;
  min->y = s->ctr.y - s->rad;
  min->z = s->ctr.z - s->rad;
  max->x = s->ctr.x + s->rad;
  max->y = s->ctr.y + s->rad;
  max->z = s->ctr.z + s->rad;

  return 1;
}

static void sphere_intersect(sphere * spr, ray * ry) {
  flt b, disc, t1, t2, temp;
  vector V;

// To eliminate the LLC (Last Level Cache) latency that
// occurs when reading spr->rad in the assignment statement
// for disc a few lines below, uncomment the following asm
// software prefetch instruction:

// asm ("prefetcht0 %0\n" : :"m"(spr->rad)); 

  VSUB(spr->ctr, ry->o, V);
  VDOT(b, V, ry->d); 
  VDOT(temp, V, V);  

  disc=b*b + spr->rad*spr->rad - temp;

  if (disc<=0.0) return;
  disc=sqrt(disc);

  t2=b+disc;
  if (t2 <= SPEPSILON) 
    return;
  add_intersection(t2, (object *) spr, ry);  

  t1=b-disc;
  if (t1 > SPEPSILON) 
    add_intersection(t1, (object *) spr, ry);  
}

static void sphere_normal(sphere * spr, vector * pnt, ray * incident, vector * N) {
  VSub((vector *) pnt, &(spr->ctr), N);

  VNorm(N);

  if (VDot(N, &(incident->d)) > 0.0)  {
    N->x=-N->x;
    N->y=-N->y;
    N->z=-N->z;
  } 
}

