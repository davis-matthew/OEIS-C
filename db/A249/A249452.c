/*
 * A249452  Numbers k such that A249441(k) = 3.
 * https://oeis.org/A249452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 15; for n ≥ 2: if n is even, a(n) = 2^(n/2 + 4) – 1 if n
 *            is odd, a(n) = 3 * 2^((n–1)/2 + 3) – 1
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The math function needed */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for n < 1, return 0 */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)15;      /* 2^4 – 1 */

    if ((n & 1) == 0) {        /* n even */
        idx_t half = n >> 1;   /* n/2   */
        seq_t v = pow2(half + 4);   /* 2^(half+4) */
        return v - 1;
    }
    else {                     /* n odd (n > 1) */
        idx_t half = (n - 1) >> 1; /* (n-1)/2 */
        seq_t v = pow2(half + 3);   /* 2^(half+3) */
        v = v * (seq_t)3 - 1;       /* 3*2^(half+3) – 1 */
        return v;
    }
}
