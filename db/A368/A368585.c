/*
 * A368585  a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * binomial(k+2,3) / k!.
 * https://oeis.org/A368585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n*a(n-1) + binomial(n+2,3); a(0) = 0
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for computed values */
static seq_t *seq_cache = NULL;  /* array of a(i) values */
static idx_t  cache_max  = 0;    /* highest index computed (inclusive) */
static size_t cache_size = 0;    /* current allocated capacity */

/* main sequence function */
seq_t a(idx_t n)
{
    /* handle n == 0 separately, ensuring cache exists */
    if (!seq_cache) {
        cache_size = 1;
        seq_cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!seq_cache) return (seq_t)0; /* allocation failure (unlikely) */
        seq_cache[0] = 0;
        cache_max = 0;
    }
    if (n == 0) return seq_cache[0];

    /* grow cache if needed */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_size * sizeof(seq_t));
        if (!tmp) return seq_cache[cache_max];  /* return last known value on failure */
        seq_cache = tmp;
        cache_size = new_size;
    }

    /* compute values from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* binomial(i+2,3) = (i+2)*(i+1)*i / 6 */
        seq_t bin = ((seq_t)(i + 2) * (i + 1) * i) / 6;
        seq_t prev = seq_cache[i - 1];
        seq_cache[i] = -(seq_t)i * prev + bin;
    }
    cache_max = n;
    return seq_cache[n];
}
