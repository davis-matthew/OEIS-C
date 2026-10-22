/*
 * A360153  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-6*k,n-3*k).
 * https://oeis.org/A360153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A360153
 *  Formula: a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-6*k, n-3*k)
 *  Recurrence used: a(n) = a(n-3) + binomial(2*n, n)
 *  Offset: 0  (a(0)=1, a(1)=2, a(2)=6)
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle very small n directly */
    switch ((int)n) {
        case 0:  return (seq_t)1;
        case 1:  return (seq_t)2;
        case 2:  return (seq_t)6;
        default: break;
    }

    /* Circular buffer for a[i-3] (size 3) */
    seq_t buf[3] = { (seq_t)1, (seq_t)2, (seq_t)6 };   /* a[0], a[1], a[2] */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t term = binomial((idx_t)(2 * i), i);       /* binomial(2*i, i) */
        seq_t a_i   = buf[(i - 3) % 3] + term;           /* a(i) = a(i-3)+term */
        buf[i % 3]  = a_i;                              /* store new value */
    }

    return buf[n % 3];
}
