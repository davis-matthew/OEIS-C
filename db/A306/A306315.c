/*
 * A306315  Number of binary words of length n such that the difference
 *          between the number of 1's and the number of 0's is in the interval
 *          [-2,3] for every prefix and in the interval [-3,2] for every
 *          suffix.
 * https://oeis.org/A306315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 11*a(n-2) -
 *            46*a(n-4) + 90*a(n-6) * - 81*a(n-8) + 28*a(n-10) - 3*a(n-12) *
 *            for n ≥ 12, with initial terms * a(0)=1, a(1)=2, a(2)=4, a(3)=6,
 *            a(4)=12, a(5)=18, * a(6)=35, a(7)=54, a(8)=103, a(9)=162,
 *            a(10)=307, a(11)=486
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 77, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)2,  (seq_t)4,  (seq_t)6,
        (seq_t)12, (seq_t)18, (seq_t)35, (seq_t)54,
        (seq_t)103,(seq_t)162,(seq_t)307,(seq_t)486
    };

    if (n < 12u)
        return init[(size_t)n];

    /* Circular buffer holding the last 12 computed values */
    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    /* Compute values from 12 up to n */
    for (idx_t cur = 12u; cur <= n; ++cur)
    {
        /* Indices modulo 12 */
        idx_t i0 = (cur - 2u) % 12u;
        idx_t i1 = (cur - 4u) % 12u;
        idx_t i2 = (cur - 6u) % 12u;
        idx_t i3 = (cur - 8u) % 12u;
        idx_t i4 = (cur -10u) % 12u;
        idx_t i5 = (cur -12u) % 12u;

        /* Compute positive and negative contributions separately
         * to avoid intermediate underflow/overflow in unsigned arithmetic. */
        seq_t pos = 11u * buf[i0];
        pos += 90u * buf[i2];
        pos += 28u * buf[i4];

        seq_t neg = 46u * buf[i1];
        neg += 81u * buf[i3];
        neg += 3u  * buf[i5];

        buf[cur % 12u] = pos - neg;
    }

    return buf[n % 12u];
}
