/*
 * A278274  Number of n X 2 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,-1) (-1,0) (-1,1) (0,-1)
 *          (0,1) or (1,0), with upper left element zero.
 * https://oeis.org/A278274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=2, a(3)=5, a(4)=16,
 * a(n)=3*a(n-1)+a(n-2)-2*a(n-3) for n>4. */
#include <stdlib.h>

/* Return the n‑th term of A278274.  The function is thread‑safe in the
 * sense that each call can be made in any order; results are cached for
 * future calls.  It works for any build where seq_t is an integer type
 * that supports +, -, *, /, and indexing, and where idx_t is an unsigned
 * integer type such as uint64_t. */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity (in terms) */

    if (n == 0)   /* offset is 1; a(0) is undefined, return 0 as safe value */
        return (seq_t)0;

    /* Return cached value if already available. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Allocate more space if necessary. */
    if (n > cache_cap) {
        seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * n);
        if (!new_cache)  /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = n;
    }

    /* Compute terms from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)0;
        else if (i == 2)
            val = (seq_t)2;
        else if (i == 3)
            val = (seq_t)5;
        else if (i == 4)
            val = (seq_t)16;
        else
            val = (seq_t)(3 * cache[i - 2] + cache[i - 3] - 2 * cache[i - 4]);

        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
