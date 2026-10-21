/*
 * A288795  a(n) = 4^n + 3^(n + 1) - 2.
 * https://oeis.org/A288795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 3^(n + 1) - 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A288795.
 * Index n starts at 1 (offset 1). */
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* 4^n  and 3^(n+1) are computed by the provided utilities.
       Since seq_t is an unsigned integer type (possibly arbitrary
       precision), normal arithmetic suffices. */
    seq_t term4 = pow4(n);          /* 4^n   */
    seq_t term3 = pow3(n + 1);      /* 3^(n+1) */
    return term4 + term3 - (seq_t)2;
}
