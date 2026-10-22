/*
 * A383538  a(n) = Sum_{k=0..floor(n/4)} binomial(n+k,k)^2 *
 *          binomial(n-3*k,k).
 * https://oeis.org/A383538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n+k,k)^2 *
 *            binomial(n-3*k,k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    idx_t m = n / 4;          // upper bound of k
    seq_t sum = 0;
    for (idx_t k = 0; k <= m; ++k) {
        seq_t b1 = binom(n + k, k);      /* binomial(n+k, k) */
        seq_t b2 = binom(n - 3 * k, k);  /* binomial(n-3k, k) */
        seq_t term = b1 * b1 * b2;       /* (binomial(n+k, k))^2 * binomial(n-3k, k) */
        sum += term;
    }
    return sum;
}
