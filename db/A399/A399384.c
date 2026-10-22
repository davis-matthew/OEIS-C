/*
 * A399384  a(n) = (2*n + 1)*(n!)^2.
 * https://oeis.org/A399384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*(n!)^2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *fact_cache = NULL;      /* cache of factorials */
static size_t fact_cap = 0;           /* allocated capacity */
static idx_t fact_len = 0;            /* highest computed factorial index + 1 */

/* Return factorial(n) using a cache. */
static seq_t cached_factorial(idx_t n)
{
    if (fact_len == 0) {
        /* initialise cache with factorial(0) = 1 */
        fact_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!fact_cache) return (seq_t)0;
        fact_cap = 1;
        fact_cache[0] = (seq_t)1;
        fact_len = 1;
    }
    if ((size_t)n >= fact_cap) {
        size_t new_cap = fact_cap ? fact_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        fact_cache = tmp;
        fact_cap = new_cap;
    }
    /* compute missing factorials */
    for (idx_t i = fact_len; i <= n; ++i) {
        fact_cache[i] = fact_cache[i - 1] * ((seq_t)i);
    }
    fact_len = n + 1;
    return fact_cache[n];
}

seq_t a(idx_t n)
{
    seq_t fact = cached_factorial(n);
    seq_t res = fact * fact;                /* (n!)^2 */
    res *= ((seq_t)2 * n + (seq_t)1);        /* multiply by (2*n + 1) */
    return res;
}
