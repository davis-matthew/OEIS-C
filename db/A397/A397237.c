/*
 * A397237  Expansion of 1/(1 - 2*x + x^2 - x^3 - 2*x^4 - x^5).
 * https://oeis.org/A397237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) + 2*a(n-4) + a(n-5); Initial
 *            values: a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=11
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t   cache_size = 0;       /* allocated size of cache */
static idx_t   last_computed = 0;    /* largest index for which cache[i] is valid */

/* ensure the cache can hold terms up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = cache_size ? cache_size : 16;
        while (new_size <= n)
            new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }
}

/* compute the n-th term of A397237 */
seq_t a(idx_t n)
{
    /* handle the first five terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)11;

    /* ensure cache is large enough */
    ensure_cache(n);

    /* initialise the cache if this is the first computation */
    if (last_computed == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)11;
        last_computed = 4;
    }

    /* extend the cache up to index n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 2*cache[i-1] - cache[i-2] + cache[i-3] + 2*cache[i-4] + cache[i-5];
    }
    last_computed = n;

    return cache[n];
}
