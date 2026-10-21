/*
 * A285122  Min(|d(k+1-i) - d(i)|, for i = 1..k), where d(1),..,d(k) are the
 *          divisors of n^2+1.
 * https://oeis.org/A285122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(|d(k+1-i) – d(i)|, for i = 1..k) where d(i) are the
 *            divisors of n^2+1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = n^2 + 1 */
    seq_t m = (seq_t)n * (seq_t)n + 1;

    seq_t minDiff = m;          /* a difference cannot exceed m-1 */

    for (idx_t d = 1; (seq_t)d * (seq_t)d <= m; ++d)
    {
        if (m % d == 0)
        {
            seq_t other = m / (seq_t)d;
            seq_t diff = (seq_t)d > other ? (seq_t)d - other : other - (seq_t)d;
            if (diff < minDiff)
                minDiff = diff;
        }
    }

    return minDiff;
}
