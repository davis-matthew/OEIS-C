/*
 * A361276  Number of 2413-avoiding even Grassmannian permutations of size n.
 * https://oeis.org/A361276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A361276: Number of 2413-avoiding even Grassmannian permutations of size n
   Recurrence (n >= 6):
       a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + a(n-4) + 2*a(n-5) - a(n-6)
   Initial values:
       a(0)=1, a(1)=1, a(2)=1, a(3)=3, a(4)=6, a(5)=13
*/
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t computed_len = 0;/* number of values computed so far */

    /* expand the cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 16) new_cap = 16;
        /* grow roughly by doubling */
        if (new_cap < cache_cap * 2) new_cap = cache_cap * 2;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        /* copy existing data if any */
        if (computed_len > 0) {
            for (idx_t i = 0; i < computed_len; ++i)
                new_cache[i] = cache[i];
        }
        /* free old storage */
        if (cache != NULL)
            seq_free_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = computed_len; i <= n; ++i) {
        if (i == 0)        cache[i] = 1;
        else if (i == 1)   cache[i] = 1;
        else if (i == 2)   cache[i] = 1;
        else if (i == 3)   cache[i] = 3;
        else if (i == 4)   cache[i] = 6;
        else if (i == 5)   cache[i] = 13;
        else {
            /* recurrence: a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + a(n-4) + 2*a(n-5) - a(n-6) */
            seq_t val = 2 * cache[i - 1];
            val += cache[i - 2];
            val -= 4 * cache[i - 3];
            val += cache[i - 4];
            val += 2 * cache[i - 5];
            val -= cache[i - 6];
            cache[i] = val;
        }
    }
    computed_len = n + 1;  /* we now have values 0 .. n inclusive */

    return cache[n];
}
