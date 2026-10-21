/*
 * A276220  Position of n^s in the joint ranking of {h} and {k^s}, where s =
 *          sqrt(2), h >= 1, k >= 2.
 * https://oeis.org/A276220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n^(1/sqrt(2))) - 1, s = sqrt(2)
 * Range:     u8 n <= 212, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>       /* for pow and sqrt */
#include <stdint.h>     /* for uint64_t, etc. */

static const double INV_SQRT2 = 0.70710678118654752440; /* 1/sqrt(2) */

seq_t a(idx_t n)
{
    /* Offset is 1 : a(1) = 1.  For n >= 1. */
    if (n == 0)    /* undefined for offset 1, return 0 for safety */
        return (seq_t)0;

    double nd = (double)n;
    /* Compute floor( n^(1/sqrt(2)) ) using double arithmetic.
       Small epsilon added to guard against floating-point round‑off
       near an integer boundary. */
    double exp_val = pow(nd, INV_SQRT2) + 1e-12;
    idx_t m = (idx_t)exp_val;   /* truncates towards zero = floor for positive numbers */

    return (seq_t)n + (seq_t)m - 1;
}
