/*
 * A291632  Column 1 of A122832.
 * https://oeis.org/A291632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n-1)*a(n) = n*a(n-1) + 2*(n-1)*n*a(n-2) with initial values
 *            a(0)=0, a(1)=1, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size   = 0;   /* allocated size of cache array      */
static idx_t  last_computed = (idx_t)(-1);  /* largest index that has been computed */

/* 
 * Compute the n-th term of OEIS A291632.
 * Works for any seq_t (uint8_t ... arbitrary‑precision) and for any idx_t.
 * Numbers that fit in seq_t are computed exactly.
 */
seq_t a(idx_t n)
{
    /* handle trivial cases immediately */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* initialise the cache once */
    if (cache_size == 0) {
        cache_size = n + 1;
        if (cache_size < 3) cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        last_computed = 2;
    }
    /* enlarge the cache if required (but keep previously computed values) */
    else if (cache_size <= n) {
        cache_size = n + 1;
        cache = (seq_t*)realloc(cache, cache_size * sizeof(seq_t));
    }

    /* compute any missing terms up to n */
    if (last_computed < n) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            /* all arithmetic is in seq_t; casts below are safe */
            seq_t term1 = cache[i-1] / ( (seq_t)(i-1) );            /* a(i‑1)/(i‑1)   */
            seq_t term2 = ((seq_t)2 * (seq_t)i) * cache[i-2];       /* 2*i*a(i‑2)     */
            cache[i] = cache[i-1] + term1 + term2;                  /* a(i‑1)+term1+term2 */
        }
        last_computed = n;
    }

    return cache[n];
}
