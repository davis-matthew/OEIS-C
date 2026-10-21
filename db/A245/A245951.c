/*
 * A245951  Number of length 1+3 0..n arrays with some pair in every
 *          consecutive four terms totalling exactly n.
 * https://oeis.org/A245951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A245951  
   Formula: a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + a(n-4) + 2*a(n-5) - a(n-6)
   Initial values: a(1)=14, a(2)=71, a(3)=196, a(4)=453, a(5)=834, a(6)=1435
   Offset: 1
*/

#include <stdlib.h>
#include <stddef.h>   /* for size_t */

/* static cache for computed values */
static seq_t *seq_cache = NULL;      /* holds a(0), a(1), ...  (index 0 unused) */
static idx_t cache_cap   = 0;        /* actual allocated size of seq_cache */
static idx_t computed_up_to = 0;     /* maximum n for which a(n) is stored */

/* Ensure that seq_cache can hold indices up to n (including n). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)        /* already big enough */
        return;

    /* grow to next power of two > n */
    idx_t new_cap = cache_cap > 0 ? cache_cap : 8;   /* at least 8 */
    while (new_cap <= n)
        new_cap <<= 1;      /* multiply by 2 */

    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache)         /* allocation failure – silently ignore, caller gets 0 */
        return;

    /* zero the new part */
    if (cache_cap < new_cap) {
        for (idx_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = 0;
    }

    seq_cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the first six terms of the sequence. */
static void initialise_base(void)
{
    ensure_cache(6);

    static const seq_t init[7] = {
        0,                          /* unused – 1‑based indexing */
        (seq_t)14, (seq_t)71, (seq_t)196, (seq_t)453,
        (seq_t)834, (seq_t)1435
    };

    for (idx_t i = 1; i <= 6; ++i)
        seq_cache[i] = init[i];

    computed_up_to = 6;
}

/* Public function: returns a(n) (n>=1). */
seq_t a(idx_t n)
{
    if (n < 1)          /* values below the offset are undefined */
        return 0;

    ensure_cache(n);

    if (computed_up_to < 6)          /* first time the cache is used */
        initialise_base();

    if (n <= computed_up_to)         /* already computed */
        return seq_cache[n];

    /* Compute forward from the last known value up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t val = 2 * seq_cache[i-1] + seq_cache[i-2]
                    - 4 * seq_cache[i-3] + seq_cache[i-4]
                    + 2 * seq_cache[i-5] - seq_cache[i-6];
        seq_cache[i] = val;
    }

    computed_up_to = n;
    return seq_cache[n];
}
