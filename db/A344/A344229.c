/*
 * A344229  a(n) = n*a(n-1) + n^signum(n mod 4), a(0) = 1.
 * https://oeis.org/A344229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + n^signum(n mod 4), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n > 0: a(n) = n*a(n-1) + (n if n%4!=0 else 1) */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for previously computed values.
   cache_len   : number of terms stored (cache[0..cache_len-1]).
   cache_cap   : capacity of the allocated array. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is large enough to hold a(0) */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();            /* graceful failure if memory allocation fails */
        cache[0] = 1;
        cache_len = 1;
    }

    /* Grow cache if needed */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap <= (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already computed */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;   /* a(0)=1, already set but keep for safety */
            continue;
        }
        seq_t prev = cache[i - 1];
        seq_t val = (seq_t)i * prev;     /* n * a(n-1) */

        /* n^signum(n mod 4) : add n if n%4 != 0, else add 1 */
        if (i % 4 != 0)
            val += i;
        else
            val += 1;

        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
