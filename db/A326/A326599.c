/*
 * A326599  G.f.: Sum_{n>=0} x^n * (1 + x^n)^n / (1 - x*(1 + x^n))^(n+1).
 * https://oeis.org/A326599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326599
   Coefficients of the generating function
   A(x) = Σ_{k≥0} x^k (1+x^k)^k / (1 - x(1+x^k))^{k+1}.
   For n ≥ 0 the coefficient a(n) can be expressed as

       a(n) = 2^n
              + Σ_{k=1}^{n} Σ_{j=0}^{min(⌊n/(k+1)⌋, ⌊(n-k)/k⌋)}
                binom(n - k*j, n - k - k*j) * binom(n - k*j, j).

   The inner sum runs over all j for which
   n = k + (n - k - k*j) + k*j  and   0 ≤ j ≤ n - k*j.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

/* 2^exponent using the provided pow_uint helper. */
static seq_t pow2(idx_t exponent) {
    return pow_uint(2, exponent);
}

/* Computes the nth term of A326599.  The function has external linkage. */
seq_t a(idx_t n) {
    /* 2^n is the contribution of the n = 0 summand in the generating function. */
    seq_t total = pow2(n);

    /* Accumulate the terms for k = 1 .. n. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Maximal j from the two constraints:
           j ≤ ⌊n/(k+1)⌋   (from j ≤ n - k*j)
           j ≤ ⌊(n-k)/k⌋   (from k = n - k - k*j ≥ 0)          */
        idx_t max_j = n / (k + 1);
        idx_t cand = (n >= k) ? (n - k) / k : 0;
        if (cand < max_j) {
            max_j = cand;
        }

        for (idx_t j = 0; j <= max_j; ++j) {
            idx_t k_val = n - k - k * j;          /* other component of the partition */
            if (k_val > n) {
                continue;   /* safety check, should not happen */
            }
            idx_t L = k + k_val;                  /* L = n - k*j = k + k_val */

            /* binomial(L, k_val) * binomial(L, j) */
            seq_t term = binom(L, k_val);
            term *= binom(L, j);

            total += term;
        }
    }

    return total;
}
