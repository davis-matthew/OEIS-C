/*
 * A329722  a(n) = Sum_{k=0..n} ((binomial(n+2k,2n-k)*binomial(n,k)) mod 2).
 * https://oeis.org/A329722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n+2k,2n-k) * binomial(n,k)) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(n,k) is odd iff (k & ~n) == 0 */
        if ((k & ~n) == 0) {
            idx_t m = n + 2 * k;     /* n + 2k */
            idx_t r = 2 * n - k;     /* 2n - k */
            /* binomial(m,r) is odd iff (r & ~m) == 0 */
            if ((r & ~m) == 0) {
                ++sum;
            }
        }
    }
    return sum;
}
