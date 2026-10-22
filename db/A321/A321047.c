/*
 * A321047  a(n) = Sum_{1<=i<j<=n} floor(n*(1/i - 1/j)).
 * https://oeis.org/A321047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1<=i<j<=n} floor(n*(1/i - 1/j)) (OEIS A321047)
 * Range:     u8 n <= 14, u16 n <= 140, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* a(n) returns the nth term (a(1)=0).  This implementation
   evaluates the sum directly in O(n²) time, which is fast enough
   for n up to about 10 000 (≈50 M iterations). */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t sum = 0;
    const seq_t sn = (seq_t)n;

    for (idx_t i = 1; i < n; ++i) {
        seq_t denom_i = (seq_t)i;
        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t num   = sn * (j - i);
            seq_t denom = denom_i * (seq_t)j;
            sum += num / denom;
        }
    }
    return sum;
}
