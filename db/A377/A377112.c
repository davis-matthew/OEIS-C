/*
 * A377112  a(n) = coefficient of sqrt(6) in the expansion of (2 + sqrt(2) +
 *          sqrt(3))^n.
 * https://oeis.org/A377112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2) - 8*a(n-3) + 23*a(n-4), initial
 *            values a(0)=0, a(1)=0, a(2)=2, a(3)=12
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* trivial base cases */
    if (n == 0u) return 0;
    if (n == 1u) return 0;
    if (n == 2u) return 2;
    if (n == 3u) return 12;

    /* keep the last four values of the recurrence */
    seq_t a0 = 0; /* a(n-4)  */
    seq_t a1 = 0; /* a(n-3)  */
    seq_t a2 = 2; /* a(n-2)  */
    seq_t a3 = 12;/* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        /* compute 8*a(n-1)  + 23*a(n-4) */
        seq_t t1 = mul_small(a3, 8);
        seq_t t4 = mul_small(a0, 23);
        seq_t sum = t1;
        sum += t4;

        /* compute 14*a(n-2) + 8*a(n-3) */
        seq_t t2 = mul_small(a2, 14);
        seq_t t3 = mul_small(a1, 8);
        seq_t sub = t2;
        sub += t3;

        /* new value of a(n) = sum - sub */
        seq_t newval = sum - sub;

        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = newval;
    }

    return a3;
}
