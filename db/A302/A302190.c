/*
 * A302190  Hurwitz logarithm of natural numbers 1,2,3,4,5,...
 * https://oeis.org/A302190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=0, a(1)=2, a(n)=(-1)^(n-1)*(n-1)! for n>=2
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Internal cache for factorial values: fact_cache[k] = k!  (k>=0) */
static seq_t *fact_cache = NULL;
static idx_t fact_cache_cap = 0;   /* allocated size in elements   */
static idx_t fact_cache_max = 0;   /* maximum k for which fact_cache[k] is valid */

/* Helper: ensure the cache is allocated large enough for indices up to 'need' */
static void ensure_fact_cache(idx_t need) {
    if (fact_cache == NULL) {
        /* allocate (need+1) factorial values, index 0..need */
        fact_cache = (seq_t *)calloc(need + 1, sizeof(seq_t));
        fact_cache[0] = (seq_t)1;
        fact_cache_cap = need + 1;
        fact_cache_max = 0;
    } else if (fact_cache_cap <= need) {
        /* grow the cache */
        fact_cache = (seq_t *)realloc(fact_cache, (need + 1) * sizeof(seq_t));
        fact_cache_cap = need + 1;
    }
}

/* Append factorials up to 'need' into the cache */
static void compute_factorials_upto(idx_t need) {
    for (idx_t k = fact_cache_max + 1; k <= need; ++k) {
        fact_cache[k] = fact_cache[k - 1] * (seq_t)k;
    }
    fact_cache_max = need;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)2;

    idx_t need = n - 1;           /* we need (n-1)! */
    ensure_fact_cache(need);
    if (fact_cache_max < need)
        compute_factorials_upto(need);

    seq_t val = fact_cache[need]; /* (n-1)! */
    /* apply the sign (-1)^(n-1) */
    if ((need & 1) != 0)   /* need is odd → negative sign */
        val = -val;
    return val;
}
