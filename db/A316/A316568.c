/*
 * A316568  Largest k such that 1^2 + n, 2^2 + n, ..., k^2 + n are composite.
 * https://oeis.org/A316568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = largest k such that 1^2+n, 2^2+n, ..., k^2+n are all composite.
 * This is equivalent to: find the smallest i≥1 for which i^2+n is prime; then a(n)=i‑1.
 * Implementation uses the provided is_prime_big routine for primality testing.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* Promote n to seq_t once for efficiency */
    seq_t n_val = (seq_t)n;

    /* Search for the first i where i^2 + n is prime */
    for (idx_t i = 1ULL; ; ++i) {
        /* Compute i^2 + n, promote i to seq_t */
        seq_t val = (seq_t)i * (seq_t)i + n_val;
        if (is_prime_big(val)) {
            /* All previous values were composite; return the count */
            return (seq_t)(i - (idx_t)1);
        }
        /* otherwise continue searching */
    }
}
