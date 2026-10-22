/*
 * A370799  Expansion of (1/x) * Series_Reversion( x/(x+1/(1-x+x^2)) ).
 * https://oeis.org/A370799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 17, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   binomial_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A370799
   Formula: a(n) = (1/(n+1)) *
                 Σ_{j=0..⌊n/4⌋} Σ_{k=0..⌊(n-4j)/3⌋}
                 binom(n+1,j) * binom(n+k,k) *
                 binom(n+1-j,3j+3k+1) * (-1)^k * 2^(n-4j-3k) */

#include <stdlib.h>
#include "../utils/binomial_mult.h"

/* cache for powers of two up to the largest exponent needed */
static seq_t *pow2_cache = NULL;
static idx_t pow2_cap = 0;
static void ensure_pow2(idx_t max_exp)
{
    if (max_exp < pow2_cap) return;
    size_t new_cap = (size_t)max_exp + 1;
    seq_t *new_cache = (seq_t *)realloc(pow2_cache, new_cap * sizeof(seq_t));
    if (pow2_cap == 0) {
        new_cache[0] = (seq_t)1;
        pow2_cap = 1;
        pow2_cache = new_cache;
    }
    for (size_t i = pow2_cap; i < new_cap; ++i) {
        new_cache[i] = new_cache[i - 1] * 2;
    }
    pow2_cache = new_cache;
    pow2_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* we need 2^e for e up to n */
    ensure_pow2(n);

    seq_t total = 0;
    idx_t max_j = n / 4;

    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t Cn1j = binomial_mult(n + 1, j);

        idx_t m = n + 1 - j;               /* for second binomial */
        idx_t r0 = 3 * j + 1;              /* initial r = 3j+1  */
        seq_t Mr = 1;                      /* C(m,r0) */
        for (idx_t i = 1; i <= r0; ++i) {
            Mr = (Mr * (seq_t)(m - r0 + i)) / (seq_t)i;
        }

        idx_t max_k = (n - 4 * j) / 3;

        seq_t ck = 1; /* C(n+k,k) with k=0 */
        for (idx_t k = 0; k <= max_k; ++k) {
            idx_t e = n - 4 * j - 3 * k;
            seq_t pow2e = pow2_cache[e];

            seq_t term = Cn1j;
            term *= ck;
            term *= Mr;
            term *= pow2e;

            if (k & 1u) term = -term;

            total += term;

            /* update for next k, if any */
            if (k != max_k) {
                ck = (ck * (seq_t)(n + k + 1)) / (seq_t)(k + 1);

                /* update Mr from r to r+3:
                   Mr' = Mr * (m-r)(m-r-1)(m-r-2) / ((r+1)(r+2)(r+3))   */
                idx_t r = 3 * j + 3 * k + 1;
                seq_t num = Mr * (seq_t)(m - r) * (seq_t)(m - r - 1) *
                            (seq_t)(m - r - 2);
                seq_t den = (seq_t)(r + 1) * (seq_t)(r + 2) * (seq_t)(r + 3);
                Mr = num / den;
            }
        }
    }

    return total / (seq_t)(n + 1);
}
