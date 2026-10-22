/*
 * A395383  a(n) = (2*n)! * [x^(2*n)] 1 / cos(x)^5.
 * https://oeis.org/A395383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = -(1/16) * Σ_{k=0}^{n-1} (-1)^(n-k) * (10 +
 *            5*9^(n-k) + 25^(n-k)) * binomial(2*n, 2*k) * a(k) The
 *            implementation memoizes results so that calls may come in any
 *            order. See OEIS entry for A395383
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache for coefficients a(k) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;
    static bool init_cache = false;

    /* static powers of 9 and 25 up to current n */
    static seq_t *pow9  = NULL;
    static seq_t *pow25 = NULL;
    static size_t pow_cap  = 0;
    static idx_t pow_max   = 0;
    static bool init_pow = false;

    /* Initialize cache for a(0) */
    if (!init_cache) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_max = 0;
        init_cache = true;
    }

    /* Ensure cache capacity up to n */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while ((idx_t)(new_cap) <= n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialize powers of 9 and 25 */
    if (!init_pow) {
        pow_cap = 1;
        pow9  = (seq_t*)malloc(pow_cap * sizeof(seq_t));
        pow25 = (seq_t*)malloc(pow_cap * sizeof(seq_t));
        pow9[0]  = (seq_t)1;
        pow25[0] = (seq_t)1;
        pow_max = 0;
        init_pow = true;
    }

    if ((idx_t)(pow_cap) <= n) {
        size_t new_cap = pow_cap ? pow_cap : 1;
        while ((idx_t)(new_cap) <= n) new_cap <<= 1;
        pow9  = (seq_t*)realloc(pow9,  new_cap * sizeof(seq_t));
        pow25 = (seq_t*)realloc(pow25, new_cap * sizeof(seq_t));
        pow_cap = new_cap;
    }

    /* Extend powers if needed */
    if (n > pow_max) {
        for (idx_t j = pow_max + 1; j <= n; ++j) {
            pow9[j]  = mul_small(pow9[j-1], 9);
            pow25[j] = mul_small(pow25[j-1], 25);
        }
        pow_max = n;
    }

    /* Compute values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1; continue; }

        seq_t pos_sum = (seq_t)0;
        seq_t neg_sum = (seq_t)0;

        /* binomial(2*i, 0) starts at 1 */
        seq_t binom = (seq_t)1;

        for (idx_t k = 0; k < i; ++k) {
            /* m = i - k */
            idx_t m = i - k;

            /* coefficient part : 10 + 5*9^m + 25^m */
            seq_t coeff = (seq_t)10;
            seq_t temp  = mul_small(pow9[m], 5);
            coeff += temp;
            coeff += pow25[m];

            /* term = coeff * binom * a(k) */
            seq_t term = seq_mul(coeff, binom);
            term      = seq_mul(term, cache[k]);

            /* accumulate with sign */
            if ((m & 1) == 1)
                neg_sum += term;
            else
                pos_sum += term;

            /* next even binomial: binom = binom * (2*i-2*k) * (2*i-2*k-1) / ((2*k+2)*(2*k+1)) */
            if (k + 1 < i) {   /* no need to update after last k */
                seq_t num = binom;
                num = mul_small(num, (uint64_t)(2*i - 2*k));
                num = mul_small(num, (uint64_t)(2*i - 2*k - 1));
                uint64_t denom = (2*k + 2) * (2*k + 1);
                binom = num / denom;
            }
        }

        /* diff = neg_sum - pos_sum   (guaranteed non‑negative) */
        seq_t diff = neg_sum;
        diff -= pos_sum;

        /* a(i) = diff / 16 */
        seq_t ai = diff / 16;
        cache[i] = ai;
    }

    cache_max = n;
    return cache[n];
}
