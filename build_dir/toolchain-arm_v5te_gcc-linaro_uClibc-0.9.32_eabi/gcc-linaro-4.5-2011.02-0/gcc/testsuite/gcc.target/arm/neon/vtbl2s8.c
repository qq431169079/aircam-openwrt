/* Test the `vtbl2s8' ARM Neon intrinsic.  */
/* This file was autogenerated by neon-testgen.  */

/* { dg-do assemble } */
/* { dg-require-effective-target arm_neon_ok } */
/* { dg-options "-save-temps -O0" } */
/* { dg-add-options arm_neon } */

#include "arm_neon.h"

void test_vtbl2s8 (void)
{
  int8x8_t out_int8x8_t;
  int8x8x2_t arg0_int8x8x2_t;
  int8x8_t arg1_int8x8_t;

  out_int8x8_t = vtbl2_s8 (arg0_int8x8x2_t, arg1_int8x8_t);
}

/* { dg-final { scan-assembler "vtbl\.8\[ 	\]+\[dD\]\[0-9\]+, \\\{((\[dD\]\[0-9\]+-\[dD\]\[0-9\]+)|(\[dD\]\[0-9\]+, \[dD\]\[0-9\]+))\\\}, \[dD\]\[0-9\]+!?\(\[ 	\]+@.*\)?\n" } } */
/* { dg-final { cleanup-saved-temps } } */
