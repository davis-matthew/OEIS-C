/*
 * A248823  Number of integers k^5 that divide 1!*2!*3!*...*n!.
 * https://oeis.org/A248823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{p≤n} (⌊v_p(P(n))/5⌋+1) where * P(n) = 1!*2!*…*n! and *
 *            v_p(P(n)) = ∑_{i=1}^{n} ∑_{k≥1} ⌊i/p^k⌋
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 32, u64 n <= 60, u128 n <= 112,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Compute the number of integers k^5 dividing 1!*2!*…*n! */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined behaviour for n<1, but handle gracefully */

    /* Get all primes ≤ n */
    uint64_t *primes = NULL;
    size_t  prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Result: product over primes of (floor(vp/5)+1) */
    seq_t result = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        idx_t vp = 0;               /* exponent of p in P(n) */

        /* d iterates over powers of p (p, p^2, p^3, ...) */
        idx_t d = p;
        while (d <= n) {
            idx_t m = n / d;                     /* floor(n/d) */

            /* sum_{i=1}^n floor(i/d) = d * m*(m-1)/2 + (n - m*d + 1)*m */
            idx_t part1 = (d * m * (m - 1)) / 2;
            idx_t part2 = (n - m * d + 1) * m;
            vp += part1 + part2;

            /* Prevent overflow in the next multiplication */
            if (d > n / p) break;
            d *= p;
        }

        seq_t factor = (seq_t)(vp / 5 + 1);
        result *= factor;
    }

    free(primes);
    return result;
}
