/*
 * A346394  Expansion of e.g.f. -log(1 - x) * exp(2*x).
 * https://oeis.org/A346394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n) = (n+1)*a(n-1) - 2*(n-1)*a(n-2) + 2^(n-1)
 *            (n≥2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* previous two terms */
    seq_t a0 = (seq_t)0;          /* a(0) */
    seq_t a1 = (seq_t)1;          /* a(1) */

    /* 2^(i-1) accumulator (start with 2^0 = 1) */
    seq_t pow2 = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i) {
        /* update 2^(i-1) */
        pow2 = (seq_t)2 * pow2;

        /* compute a(i) */
        seq_t term1 = (seq_t)(i + 1) * a1;
        seq_t term2 = (seq_t)2 * (seq_t)(i - 1) * a0;
        seq_t next  = term1 - term2 + pow2;

        /* shift terms for next iteration */
        a0 = a1;
        a1 = next;
    }

    return a1;
}
