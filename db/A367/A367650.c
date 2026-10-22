/*
 * A367650  a(n) = 12345*10^n + 6789.
 * https://oeis.org/A367650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 12345 * 10^n + 6789
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (recurrence: a(n) = 11*a(n-1) - 10*a(n-2),   a(0)=19134, a(1)=130239) */

#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t e) */

seq_t a(idx_t n)
{
    /* Compute 12345 * 10^n + 6789, using the provided pow10 routine. */
    seq_t term_power = pow10(n);                /* 10^n */
    seq_t result = (seq_t)12345 * term_power + (seq_t)6789;
    return result;
}
