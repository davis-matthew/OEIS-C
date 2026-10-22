/*
 * A323267  The sequence denoted by q_n used in the calculation of A323260.
 * https://oeis.org/A323267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = 3*a(n-1)+6*a(n-2)-6*a(n-3)-6*a(n-4)+8*a(n-5)+5*a(n-6)-a(n-7)
   for n > 7, with a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=5, a(6)=19, a(7)=82 */

#include <stdlib.h>
#include <string.h>

/* Cached values of the sequence.  cache[0] = a(1), cache[1] = a(2), … */
static seq_t *cache = NULL;
static size_t cache_cap = 0;          /* number of allocated elements          */
static idx_t  computed_max = 0;       /* greatest n for which a(n) is cached  */

/* Ensure the cache is large enough to hold at least n entries. */
static void ensure_cache_len(idx_t n)
{
    if (n <= cache_cap)
        return;

    seq_t *new_cache = (seq_t *)malloc(n * sizeof(seq_t));
    if (!new_cache)           /* allocation failure: terminate (safe for tests) */
        abort();

    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }

    /* If the array grows, the new slots contain indeterminate values;
       they will be fully overwritten by the recurrence before use.   */
    cache      = new_cache;
    cache_cap  = n;
}

/* The seven initial terms, stored once for every call. */
static const seq_t base[7] = {0, 0, 0, 1, 5, 19, 82};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    ensure_cache_len(n);

    /* Make sure the first seven terms are defined. */
    for (idx_t i = 0; i < 7 && i < n; ++i)
        cache[i] = base[i];

    if (computed_max < 7)
        computed_max = 7;               /* the recurrence needs 7 previous terms */

    if (n <= computed_max)
        return cache[n - 1];

    /* Compute missing terms up to n, using 0‑based array indices. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t val = 3 * cache[i - 2]        /* a(i-1) */
                  + 6 * cache[i - 3]        /* a(i-2) */
                  - 6 * cache[i - 4]        /* a(i-3) */
                  - 6 * cache[i - 5]        /* a(i-4) */
                  + 8 * cache[i - 6]        /* a(i-5) */
                  + 5 * cache[i - 7]        /* a(i-6) */
                  - cache[i - 8];           /* a(i-7) */
        cache[i - 1] = val;
    }

    computed_max = n;
    return cache[n - 1];
}
