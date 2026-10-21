/*
 * A269668  Smallest k >= 0 such that neither (k + 1)*n - k nor (k + 1)*n + k
 *          is prime.
 * https://oeis.org/A269668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 0 such that neither (k+1)*n - k nor (k+1)*n
 *            + k is prime
 * Range:     u8 n <= 28, u16 n <= 9276, u32 n <= 9276, u64 n <= 9276,
 *            u128 n <= 9276, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset handling: a(1) corresponds to n = 1 */
    seq_t nn = (seq_t)n;
    for (idx_t k = 0; ; ++k) {
        seq_t term1 = ((seq_t)(k + 1)) * nn - (seq_t)k;
        seq_t term2 = ((seq_t)(k + 1)) * nn + (seq_t)k;
        if (!is_prime_big(term1) && !is_prime_big(term2))
            return (seq_t)k;            /* found the smallest k with both composite */
    }
}
