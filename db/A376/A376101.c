/*
 * A376101  Expansion of e.g.f. -LambertW(-x / (1 - 3*x)).
 * https://oeis.org/A376101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} 3^(n-k) * k^(k-1) *
 *            binomial(n-1,k-1)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   seq_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_realloc.h"

static seq_t *fact_cache = NULL;   /* n!     , fact_cache[i] = i! */
static seq_t *pow3_cache = NULL;   /* 3^i    , pow3_cache[i] = 3^i */
static seq_t *a_cache    = NULL;   /* a(n)   */
static idx_t  cache_len_fact = 0;  /* current size of fact_cache and pow3_cache */
static idx_t  cache_len_a    = 0;  /* current size of a_cache   */

/* Ensure fact_cache[0..n] and pow3_cache[0..n] are allocated and filled. */
static void ensure_fact_p3(idx_t n)
{
    if (n < cache_len_fact) return;

    size_t new_len = n + 1;
    fact_cache = seq_realloc(fact_cache, cache_len_fact, new_len);
    pow3_cache = seq_realloc(pow3_cache, cache_len_fact, new_len);

    if (cache_len_fact == 0) {
        fact_cache[0] = (seq_t)1;
        pow3_cache[0] = (seq_t)1;
        cache_len_fact = 1;
    }
    for (idx_t i = cache_len_fact; i <= n; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
        pow3_cache[i] = pow3_cache[i-1] * (seq_t)3;
    }
    cache_len_fact = (idx_t)new_len;
}

/* Ensure a_cache[0..n] is allocated and zero‑initialised. */
static void ensure_a_cache(idx_t n)
{
    if (n < cache_len_a) return;

    size_t new_len = n + 1;
    a_cache = seq_realloc(a_cache, cache_len_a, new_len);
    for (idx_t i = cache_len_a; i <= n; ++i) a_cache[i] = (seq_t)0;
    cache_len_a = (idx_t)new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    ensure_a_cache(n);
    if (a_cache[n] != (seq_t)0) return a_cache[n];

    ensure_fact_p3(n);

    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;  /* C(n-1,0) */

    for (idx_t k = 1; k <= n; ++k) {

        seq_t term = pow3_cache[n - k];                                  /* 3^(n-k)   */
        seq_t powk = seq_pow((seq_t)k, k - 1);                           /* k^(k-1)   */
        term = term * powk;                                              /* 3^(n-k) * k^(k-1) */
        term = term * binom;                                             /* multiply by binomial */

        seq_t mult = fact_cache[n] / fact_cache[k];                     /* n! / k!   */
        term = term * mult;                                             /* full term */

        sum = sum + term;                                               /* accumulate */

        if (k < n) {                                                    /* update binomial to C(n-1,k) */
            binom = binom * (seq_t)(n - k) / (seq_t)k;
        }
    }

    a_cache[n] = sum;
    return sum;
}
