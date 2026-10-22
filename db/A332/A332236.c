/*
 * A332236  E.g.f.: -log(2 - 1 / (1 + LambertW(-x))).
 * https://oeis.org/A332236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n≥2: a(n) = n^n + Σ_{k=1}^{n-1} C(n‑1,k‑1) *
 *            (n‑k)^(n‑k) * a(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/ensure_cache.h"

/* static cache for a(n) and for k^k   */
static seq_t *a_cache = NULL;            /* a_cache[0] unused, a_cache[1] = 1 */
static size_t a_cap   = 0;              /* capacity of a_cache */
static idx_t  a_max   = 0;              /* largest n computed */

static seq_t *pow_cache = NULL;          /* pow_cache[k] = k^k */
static size_t pow_cap  = 0;              /* capacity of pow_cache */
static idx_t  pow_max  = 0;              /* largest k computed */

/* Helper to grow power cache */
static void ensure_pow_cache(idx_t n)
{
    if (n <= pow_max) return;

    size_t new_cap = pow_cap ? pow_cap : 1;
    while (new_cap < (size_t)(n + 1)) new_cap <<= 1;

    seq_t *new_mem = (seq_t *)realloc(pow_cache,
                                      (new_cap + 1) * sizeof(seq_t));
    if (!new_mem) exit(EXIT_FAILURE);
    pow_cache = new_mem;
    pow_cap   = new_cap;

    for (idx_t i = pow_max + 1; i <= n; ++i)
        pow_cache[i] = big_pow((seq_t)i, (idx_t)i);
    pow_max = n;
}

/* Main routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used for the sequence */

    /* ensure cache for a_cache */
    ensure_cache(n, &a_cache, &a_cap);
    /* pre‑compute needed powers k^k */
    ensure_pow_cache(n);

    /* compute all missing values up to n */
    for (idx_t v = a_max + 1; v <= n; ++v)
    {
        /* first term v^v */
        seq_t term = big_pow((seq_t)v, (idx_t)v);

        /* sum over k = 1 .. v-1 of binom(v-1, k-1) * (v-k)^(v-k) * a(k) */
        seq_t sum = 0;
        seq_t binom = 1;                 /* binom(v-1,0) */
        for (idx_t k = 1; k < v; ++k)
        {
            idx_t j = v - k;             /* j = v-k   */
            seq_t prod = binom * pow_cache[j];
            prod *= a_cache[k];
            sum += prod;

            /* update binom for next k+1: binom(n-1,k) = binom(n-1,k-1) * (n-1-(k-1))/k */
            if (k < v - 1)
                binom = (binom * (seq_t)(v - k)) / (seq_t)k;
        }

        a_cache[v] = term + sum;
    }

    a_max = n;
    return a_cache[n];
}
