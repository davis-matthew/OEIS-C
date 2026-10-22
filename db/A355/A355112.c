/*
 * A355112  Expansion of e.g.f. 4 / (5 - 4*x - exp(4*x)).
 * https://oeis.org/A355112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A355112: Expansion of e.g.f. 4/(5-4*x-exp(4*x))
   Recurrence: a(0)=1;
   a(n) = n*a(n-1) + Σ_{k=1..n} binomial(n,k)*4^(k-1)*a(n-k)   for n≥1
*/

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t etc. */
#include <stdlib.h>   /* malloc, free, memset */

/* Memoised array of a(n) values */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;

/* Pre‑computed powers of 4: pow4[i] = 4^i   (i ≥ 0) */
static seq_t *pow4  = NULL;
static size_t pow_cap = 0;

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    /* --- Ensure cache large enough for 0..n --- */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;          /* need indices 0..n */
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (!new_cache) {                         /* allocation failed */
            return (seq_t)0;
        }
        /* Initialise new slots to 0 */
        memset(new_cache, 0, sizeof(seq_t) * new_cap);
        /* Copy old data */
        if (cache != NULL) {
            size_t copy_len = cache_cap < new_cap ? cache_cap : new_cap;
            for (size_t i = 0; i < copy_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* --- Ensure pow4 large enough for 4^(n-1) --- */
    if (n > 0 && (size_t)(n-1) >= pow_cap) {
        size_t new_pow_cap = (size_t)n;        /* need indices 0..n-1  */
        seq_t *new_pow = (seq_t *)malloc(sizeof(seq_t) * new_pow_cap);
        if (!new_pow) {                         /* allocation failed */
            return (seq_t)0;
        }
        memset(new_pow, 0, sizeof(seq_t) * new_pow_cap);
        /* Copy old powers */
        if (pow4 != NULL) {
            size_t copy_len = pow_cap < new_pow_cap ? pow_cap : new_pow_cap;
            for (size_t i = 0; i < copy_len; ++i)
                new_pow[i] = pow4[i];
            free(pow4);
        }
        /* Make sure pow4[0]==1 and fill remaining */
        if (new_pow[0] == 0)
            new_pow[0] = (seq_t)1;
        for (size_t i = 1; i < new_pow_cap; ++i)
            new_pow[i] = new_pow[i-1] * (seq_t)4;
        pow4 = new_pow;
        pow_cap = new_pow_cap;
    }

    /* --- Base case a(0) = 1 --- */
    if (n == 0) {
        if (cache[0] == 0)
            cache[0] = (seq_t)1;
        return cache[0];
    }

    /* --- Compute a(i) for all i ≤ n that are not yet known --- */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0)
            continue;   /* already computed */

        seq_t sum = 0;
        seq_t binom = 1;   /* binomial(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* Update binomial coefficient to binomial(i,k) */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            /* term = binom * 4^(k-1) * a(i-k) */
            seq_t term = binom * pow4[k-1];
            term = term * cache[i - k];
            sum += term;
        }

        seq_t prod = (seq_t)i * cache[i - 1];
        cache[i] = prod + sum;
    }

    return cache[n];
}
