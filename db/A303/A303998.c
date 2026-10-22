/*
 * A303998  Number of ways to write 2*n+1 as p + 2^k + binomial(2*m,m), where
 *          p is a prime, and k and m are positive integers.
 * https://oeis.org/A303998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to represent 2*n+1 as p + 2^k +
 *            binomial(2*m, m), where p is prime, k>0 and m>0
 * Range:     u8 n <= 9, u16 n <= 2509, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions are defined in the utils library. */
#include <stdbool.h>
#include "../utils/binom.h"          /* provides seq_t binom(idx_t a, idx_t b); */
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for safety return 0 otherwise. */
    if (n < 1) return 0;

    /* Target value X = 2*n + 1.  Compute in seq_t to avoid overflow. */
    seq_t X = 2 * ((seq_t)n) + 1;

    seq_t count = 0;

    /* Loop over integer m >= 1 while binomial(2*m, m) <= X. */
    for (idx_t m = 1;; ++m) {
        seq_t C = binom(2 * m, m);     /* binomial(2m, m) */
        if (C > X) break;              /* larger m cannot satisfy the equation */
        if (C == 0) break;             /* overflow sign; safe exit           */

        seq_t max_pow = X - C;          /* maximum possible value of 2^k   */

        /* Start with the smallest power of two allowed (2^1). */
        seq_t pow2k = 2;
        while (pow2k <= max_pow) {
            seq_t p = X - C - pow2k;    /* candidate prime part          */
            if (p >= 2 && is_prime_big(p))
                ++count;                /* one valid representation       */

            /* Advance to next power of two, avoiding overflow. */
            if (pow2k > max_pow / 2) break;
            pow2k <<= 1;
        }
    }

    return count;
}
