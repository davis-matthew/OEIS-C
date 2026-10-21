/*
 * A242046  Least integer k > n + 1 such that n^2 + (n + 1)^2 + k^2 is prime.
 * https://oeis.org/A242046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer k > n + 1 such that n^2 + (n + 1)^2 + k^2
 *            is prime
 * Range:     u8 n <= 3, u16 n <= 132, u32 n <= 979, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* pre‑compute the constant part n^2 + (n+1)^2 */
    seq_t n_sq      = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t n1_sq     = (seq_t)(n + 1) * (seq_t)(n + 1); /* (n+1)^2 */
    seq_t base_sum  = n_sq + n1_sq;                 /* n^2 + (n+1)^2 */

    idx_t k = n + 2;   /* k must be strictly greater than n+1 */

    while (1) {
        seq_t k_sq  = (seq_t)k * (seq_t)k;   /* k^2 */
        seq_t sum   = base_sum + k_sq;       /* n^2 + (n+1)^2 + k^2 */

        if (is_prime_big(sum))
            return (seq_t)k;

        ++k;
    }
}
