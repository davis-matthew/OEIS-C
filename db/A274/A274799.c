/*
 * A274799  Number of n X 4 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (-2,-1) (0,-1) or (-1,0) and new values introduced
 *          in order 0..2.
 * https://oeis.org/A274799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A274799: Number of 0..2 arrays with certain offsets
   Recurrence (empirical):  a(n) = 2*a(n-1) + 15*a(n-2) + 11*a(n-3)
                            - 2*a(n-4) - 2*a(n-5)
   Initial values:  a(1) = 4, a(2) = 22, a(3) = 112, a(4) = 596, a(5) = 3104
*/
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *seq_cache   = NULL;   /* 1‑based cached values */
static size_t cache_cap   = 0;      /* allocated capacity (>= last index + 1) */
static idx_t  last_computed = 0;     /* largest n whose value is stored */

/* first five terms */
static const seq_t init_vals[5] = {
    (seq_t)4, (seq_t)22, (seq_t)112, (seq_t)596, (seq_t)3104
};

/* grow the cache to fit indices up to `need` (inclusive) */
static void grow_cache(idx_t need)
{
    if (cache_cap == 0) {
        /* first allocation */
        cache_cap = need + 1;
#if defined(__cplusplus)
        seq_cache = reinterpret_cast<seq_t*>(malloc(cache_cap * sizeof(seq_t)));
#else
        seq_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!seq_cache) abort(); /* allocation failure */
    } else if ((size_t)need + 1 > cache_cap) {
        /* increase capacity */
        size_t new_cap = cache_cap;
        while ((size_t)need + 1 > new_cap)
            new_cap *= 2;
#if defined(__cplusplus)
        seq_t *tmp = reinterpret_cast<seq_t*>(realloc(seq_cache, new_cap * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort(); /* allocation failure */
        seq_cache = tmp;
        cache_cap = new_cap;
    }
}

/* initialise cache with the first five terms */
static void init_cache(void)
{
    grow_cache(5);
    for (idx_t i = 1; i <= 5; ++i)
        seq_cache[i] = init_vals[i - 1];
    last_computed = 5;
}

/* compute a(n) for any n ≥ 1 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* out of range; not expected */

    if (n <= 5) return init_vals[n - 1];

    if (!seq_cache) init_cache();

    if (n > last_computed) {
        grow_cache(n);
        /* compute missing terms up to n */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t val = 0;
            val += (seq_t)2 * seq_cache[i - 1];
            val += (seq_t)15 * seq_cache[i - 2];
            val += (seq_t)11 * seq_cache[i - 3];
            val -= (seq_t)2 * seq_cache[i - 4];
            val -= (seq_t)2 * seq_cache[i - 5];
            seq_cache[i] = val;
        }
        last_computed = n;
    }
    return seq_cache[n];
}
