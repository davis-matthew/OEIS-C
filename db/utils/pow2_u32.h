/* pow2_u32: 2^n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_POW2_U32_H
#define OEISC_UTILS_POW2_U32_H
#include <stdint.h>

/* Computes 2^k for an arbitrary unsigned integer type seq_t.
   Works for fixed-width types and for arbitrary precision integer
   classes that overload the * operator. */
static seq_t pow2_u32(unsigned int k)
{
    seq_t r = 1;          /* start with 2^0 = 1 */
    for (unsigned int i = 0; i < k; ++i)
        r = r * 2;        /* multiply by 2 k times */
    return r;
}

#endif
