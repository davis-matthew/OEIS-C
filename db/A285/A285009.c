/*
 * A285009  Subset sums (see Comments).
 * https://oeis.org/A285009
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (8 + (n-2)*(3*n+1))/2
 * Range:     u8 n <= 10, u16 n <= 193, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is defined for n ≥ 3.  For n < 3 the function may return 0. */
#pragma once
#include <stdint.h>

/* seq_t and idx_t are defined by the build system.  No other types required. */
seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)0;            /* offset is not defined below 3 */

    seq_t nn = (seq_t)n;           /* promote to seq_t for arbitrary precision */
    seq_t term = (nn - (seq_t)2) * ((seq_t)3 * nn + (seq_t)1);
    return ((seq_t)8 + term) / ((seq_t)2);
}
