/*
 * A256322  Number of partitions of 7n into exactly 3 parts.
 * https://oeis.org/A256322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-4)-a(n-5)+a(n-6) Initial values:
 *            a(0)=0, a(1)=4, a(2)=16, a(3)=37, a(4)=65, a(5)=102
 * Range:     u8 n <= 7, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first six terms directly */
    if (n == 0) return 0;
    if (n == 1) return 4;
    if (n == 2) return 16;
    if (n == 3) return 37;
    if (n == 4) return 65;
    if (n == 5) return 102;

    /* a sliding window for the last six computed values:
       prev[0] = a(i-6), prev[1] = a(i-5), ..., prev[5] = a(i-1)  */
    seq_t prev[6] = {0, 4, 16, 37, 65, 102};
    seq_t val = 0;

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(i) from the recurrence */
        val = prev[5] + prev[4] - prev[2] - prev[1] + prev[0];

        /* shift the window: drop a(i-6) and append a(i) */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = val;
    }

    return val;
}
