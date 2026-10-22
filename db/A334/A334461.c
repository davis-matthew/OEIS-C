/*
 * A334461  a(n) is the number of partitions of n into consecutive parts that
 *          differ by 4.
 * https://oeis.org/A334461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into consecutive parts that
 *            differ by 4 (OEIS A334461)
 * Range:     u8 n <= 1533, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 follows the OEIS offset.  For safety, return 0 for n==0. */
    if (n == 0) return 0;

    seq_t count = 0;

    for (idx_t k = 1; ; ++k) {
        /* Use 128-bit ints to avoid overflow when checking the minimal sum
           k*(2*k-1).  If the minimal sum exceeds n, no larger k can work. */
        __uint128_t kval   = k;
        __uint128_t minSum = kval * (( (__uint128_t)2 * kval) - 1);
        if (minSum > (__uint128_t)n) break;

        /* For a given k, compute a_num = n + 2*k*(k-1) */
        __uint128_t num = (__uint128_t)n + (__uint128_t)2 * kval * (kval - 1);

        /* Check divisibility */
        if (num % kval == 0) {
            /* a = num / k must be integer */
            __uint128_t a_uint128 = num / kval;

            /* Convert to seq_t for comparison */
            seq_t a = (seq_t)a_uint128;

            /* Ensure the last part is positive: a - 4*(k-1) >= 1
               which is equivalent to a >= 4*(k-1)+1. */
            seq_t minLast = (seq_t)4 * (k - 1) + 1;

            if (a >= minLast) {
                ++count;
            }
        }
    }

    return count;
}
