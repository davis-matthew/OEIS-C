/*
 * A299909  Coordination sequence of node of type 3^6 in 3-uniform tiling
 *          #3.54 in the Galebach listing.
 * https://oeis.org/A299909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) - a(n-2) + a(n-3) + a(n-7) - a(n-8) + a(n-9) -
 *            a(n-10) Initial values:
 *            a(0)=1,a(1)=6,a(2)=12,a(3)=18,a(4)=24,a(5)=24,a(6)=30,a(7)=42,
 *            a(8)=48,a(9)=48,a(10)=54
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* first 11 terms are known a priori */
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)6,  (seq_t)12, (seq_t)18, (seq_t)24,
        (seq_t)24, (seq_t)30, (seq_t)42, (seq_t)48, (seq_t)48, (seq_t)54
    };

    if (n <= 10u)   /* simple bound: 10 fits in all idx_t types */
        return init[(size_t)n];

    /* Small circular buffer holding the last 11 computed values */
    seq_t prev[11];
    for (size_t j = 0; j < 11; ++j)
        prev[j] = init[j];

    seq_t newv;
    for (idx_t i = 11; i <= n; ++i)
    {
        /* recurrence for a(i) */
        newv = prev[10] - prev[9] + prev[8] + prev[4] - prev[3] + prev[2] - prev[1];

        /* shift the buffer left */
        for (size_t j = 0; j < 10; ++j)
            prev[j] = prev[j + 1];
        prev[10] = newv;
    }
    return prev[10];
}
