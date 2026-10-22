/*
 * A383873  a(n) = 3*a(n-1) - 2*a(n-2) + 5*a(n-3) starting with 1, 2, 3.
 * https://oeis.org/A383873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 5*a(n-3), with a(0)=1, a(1)=2,
 *            a(2)=3 (OEIS sequence A383873)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation array */
static idx_t cache_size = 0;  /* number of already computed terms */

static void ensure_cache(idx_t n)
{
    /* We need values up to index n inclusive. */
    idx_t new_size = n + 1;
    if (new_size <= cache_size)
        return;                      /* already sufficient */

    /* Reallocate the cache; cast the void* for C++ compatibility. */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                     /* allocation failure is fatal */
    cache = tmp;

    /* Compute new terms, starting with the index after the previous size. */
    for (idx_t i = cache_size; i < new_size; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)2;
        else if (i == 2)
            cache[i] = (seq_t)3;
        else
            cache[i] = (seq_t)3 * cache[i-1]
                     - (seq_t)2 * cache[i-2]
                     + (seq_t)5 * cache[i-3];
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    assert(n >= 0);
    ensure_cache(n);
    return cache[n];
}
