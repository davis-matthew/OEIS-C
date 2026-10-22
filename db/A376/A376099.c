/*
 * A376099  Expansion of e.g.f. -LambertW(-3*x / (1 - x))/3.
 * https://oeis.org/A376099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} (3k)^(k-1) * C(n-1,k-1) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

static seq_t *fact_cache = NULL;   /* fact_cache[i] = i!   */
static size_t fact_size  = 0;      /* current allocated size */

static seq_t *pow_cache  = NULL;   /* pow_cache[k] = (3*k)^(k-1) */
static size_t pow_size   = 0;      /* current allocated size */

/* Ensure factorial array is built up to n */
static void ensure_fact(idx_t n)
{
    if (fact_cache == NULL) {
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!fact_cache) return;           /* out of memory */
        fact_cache[0] = 1;
        fact_size = 1;
    }
    if ((size_t)n >= fact_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!tmp) return;                  /* out of memory */
        fact_cache = tmp;
        for (size_t i = fact_size; i < new_size; ++i) {
            seq_t val = (seq_t)i;
            fact_cache[i] = fact_cache[i - 1] * val;
        }
        fact_size = new_size;
    }
}

/* Ensure (3*k)^(k-1) values are built up to n */
static void ensure_power(idx_t n)
{
    if (pow_cache == NULL) {
        pow_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!pow_cache) return;           /* out of memory */
        pow_cache[0] = 0;                 /* unused slot */
        pow_size = 1;
    }
    if ((size_t)n >= pow_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(pow_cache, new_size * sizeof(seq_t));
        if (!tmp) return;                  /* out of memory */
        pow_cache = tmp;
        for (idx_t i = (idx_t)pow_size; i <= n; ++i) {
            seq_t base = (seq_t)(3ULL * (uint64_t)i);
            seq_t exp  = (seq_t)(i - 1);
            pow_cache[i] = big_pow(base, exp);
        }
        pow_size = new_size;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) = 0 */

    ensure_fact(n);
    ensure_power(n);

    seq_t result     = 0;
    seq_t cur_binom  = 1;                 /* C(n-1,0) */
    seq_t q          = fact_cache[n];     /* n!  */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t term = pow_cache[k] * cur_binom; /* (3k)^(k-1) * C(n-1,k-1) */
        term *= q;                            /* * (n!/k!)  */
        result += term;

        if (k < n) {
            /* Update binomial coefficient to C(n-1,k) */
            cur_binom = cur_binom * (seq_t)(n - k) / (seq_t)k;

            /* Update q to fact[n]/fact[k+1] */
            q = q / (seq_t)(k + 1);
        }
    }
    return result;
}
