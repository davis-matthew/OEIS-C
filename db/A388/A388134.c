/*
 * A388134  a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(3*n+1,k).
 * https://oeis.org/A388134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(3*n+1,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow3.h"
#include "../utils/binomial.h"

/* Computes the nth term of sequence A388134 */
seq_t a(idx_t n)
{
    /* 3^(n-k) for the current k, starting with k = 0 → 3^n */
    seq_t power = pow3(n);

    /* Binomial iterates: binom(n,0) = 1, binom(3n+1,0) = 1 */
    seq_t binom_n   = 1;
    seq_t binom_3n  = 1;

    seq_t result = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* result += 3^(n-k) * binom(n,k) * binom(3*n+1,k) */
        result += power * binom_n * binom_3n;

        if (k == n) break;

        /* Update binom(n,k+1) = binom(n,k) * (n-k) / (k+1) */
        binom_n = binom_n * (seq_t)(n - k) / (k + 1);

        /* Update binom(3n+1,k+1) = binom(3n+1,k) * (3n+1-k)/(k+1) */
        binom_3n = binom_3n * (seq_t)(3 * n + 1 - k) / (k + 1);

        /* Reduce power of 3 for next k: 3^(n-(k+1)) = 3^(n-k)/3 */
        power /= 3;
    }

    return result;
}
