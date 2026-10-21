/*
 * A291506  a(n) = (n!)^8 * Sum_{i=1..n} 1/i^8.
 * https://oeis.org/A291506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A291506:  a(n) = (n!)^8 * Sum_{i=1..n} 1/i^8
 * Recurrence: a(0)=0, a(1)=1,
 *             a(n+1) = (n^8 + (n+1)^8) * a(n) - n^16 * a(n-1)  (n>0)        */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* k = i-1 corresponds to the "n" in the recurrence formula */
        seq_t k      = (seq_t)(i - 1);
        seq_t k2     = k * k;
        seq_t k4     = k2 * k2;
        seq_t k8     = k4 * k4;
        seq_t k16    = k8 * k8;

        seq_t i2     = (seq_t)i * (seq_t)i;
        seq_t i4     = i2 * i2;
        seq_t i8     = i4 * i4;

        cur = (k8 + i8) * prev1 - k16 * prev2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
