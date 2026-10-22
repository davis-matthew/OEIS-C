/*
 * A386452  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} binomial(k+1,2) * a(k) *
 *          a(n-1-k).
 * https://oeis.org/A386452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; 
 *          a(n) = a(n-1) + sum_{k=0..n-1} binomial(k+1,2) * a(k) * a(n-1-k)
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: triangular number t(k) = (k+1)*k/2, computed in seq_t */
static inline seq_t triangular(idx_t k)
{
    /* cast operands to seq_t to keep type consistent */
    return ((seq_t)(k + 1) * (seq_t)k) / 2;
}

/* static cache of computed values */
static seq_t *cache = NULL;      /* pointer to array of seq_t */
static size_t cache_len = 0;     /* allocated size of cache */
static idx_t  computed_max = 0;  /* largest n for which a(n) is computed */

/* public function */
seq_t a(idx_t n)
{
    /* grow cache if necessary */
    if (cache_len <= (size_t)n)
    {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
        if (tmp == NULL) /* allocation failure – abort */
        {
            /* In a contest setting you might abort or return 0 */
            abort();
        }
        cache = tmp;
        cache_len = (size_t)n + 1;
    }

    /* initialise a(0) on first use */
    if (cache_len == 1 && computed_max == 0 && cache[0] == 0)
    {
        cache[0] = 1;    /* a(0) = 1 */
        computed_max = 0;
    }

    /* if value already computed, just return it */
    if (n <= computed_max)
        return cache[n];

    /* compute all values up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k)
        {
            seq_t term = triangular(k);
            term = term * cache[k] * cache[i - 1 - k];
            sum += term;
        }
        cache[i] = cache[i - 1] + sum;
    }

    computed_max = n;
    return cache[n];
}
