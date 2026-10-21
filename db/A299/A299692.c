/*
 * A299692  a(n) is the total area that is visible in the perspective view of
 *          the stepped pyramid with n levels described in A245092.
 * https://oeis.org/A299692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^n sigma(i) + n*(n+1) where sigma(i) = sum of
 *            divisors of i, A024916(n) = sum_{i=1}^n sigma(i), and A002378(n)
 *            = n*(n+1)
 * Range:     u8 n <= 11, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe for out-of-range calls */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        idx_t cnt = n / d;          /* how many multiples of d up to n */
        sum += (seq_t)d * (seq_t)cnt;
    }
    seq_t oblong = (seq_t)n * ((seq_t)n + 1);
    return sum + oblong;
}
