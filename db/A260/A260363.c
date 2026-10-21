/*
 * A260363  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000011 or 00001111.
 * https://oeis.org/A260363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 28, u64 n <= 62, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = a(n-1) - a(n-2) + 5*a(n-3) - a(n-4) + 5*a(n-5) - 3*a(n-6) - a(n-7)  (n > 7)
   Initial values: a(1)=48, a(2)=78, a(3)=208, a(4)=436, a(5)=616, a(6)=1378, a(7)=2880 */

#include <stdlib.h>

/* Cache of already computed values, 1‑indexed.  cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;  /* highest index currently stored (>=1 after first init) */

/* Initial seven values (index 1..7). */
static const seq_t init_vals[7] = {
    (seq_t)48,    /* a(1) */
    (seq_t)78,    /* a(2) */
    (seq_t)208,   /* a(3) */
    (seq_t)436,   /* a(4) */
    (seq_t)616,   /* a(5) */
    (seq_t)1378,  /* a(6) */
    (seq_t)2880   /* a(7) */
};

/* Ensure that cache can hold indices up to 'up_to'. */
static void grow_cache(idx_t up_to)
{
    if (up_to <= (idx_t)cache_cap)
        return;

    /* Reallocate (or allocate) the cache array.  If cache is NULL,
       this behaves like malloc. */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(up_to + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();                                   /* out of memory */
    cache = new_cache;

    /* Compute values that are newly reachable.  The loop starts from the
       first index that has not yet been computed. */
    for (idx_t i = (idx_t)(cache_cap + 1); i <= up_to; ++i)
    {
        if (i <= 7)
        {
            cache[i] = init_vals[(size_t)(i - 1)];
        }
        else
        {
            seq_t val = cache[i - 1];                           /* a(n-1)   */
            val -= cache[i - 2];                               /* -a(n-2)  */
            val += (seq_t)5 * cache[i - 3];                    /* +5a(n-3) */
            val -= cache[i - 4];                               /* -a(n-4)  */
            val += (seq_t)5 * cache[i - 5];                    /* +5a(n-5) */
            val -= (seq_t)3 * cache[i - 6];                    /* -3a(n-6) */
            val -= cache[i - 7];                               /* -a(n-7)  */
            cache[i] = val;
        }
    }

    cache_cap = (size_t)up_to;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n < 1 */

    grow_cache(n);
    return cache[n];
}
