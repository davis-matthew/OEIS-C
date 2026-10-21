/*
 * A290938  Number of dominating sets in the n-gear graph.
 * https://oeis.org/A290938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 12*a(n-2) - 2*a(n-3) + 3*a(n-4) * + 3*a(n-5) +
 *            2*a(n-6) * Initial values: a(1)=5, a(2)=23, a(3)=83, a(4)=291, *
 *            a(5)=1015, a(6)=3539
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_cap   = 0;   /* allocated size of cache array */
    static idx_t  cache_size  = 0;   /* largest index that has a value */

    /* Guard against invalid index: domain for the sequence is n ≥ 1 */
    if (n == 0)
        return (seq_t)0;          /* arbitrary choice for out‑of‑domain */

    /* Expand cache if necessary */
    if (n >= cache_cap)
    {
        idx_t new_cap = n + 1;    /* keep index n reachable */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise the first six values on first use */
    if (cache_size == 0)
    {
        if (cache_cap < 7) /* needs space for indices 1..6 */
        {
            idx_t new_cap = 7;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)23;
        cache[3] = (seq_t)83;
        cache[4] = (seq_t)291;
        cache[5] = (seq_t)1015;
        cache[6] = (seq_t)3539;
        cache_size = 6;
    }

    /* Compute missing values up to n */
    if (n > cache_size)
    {
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t t1 = 7 * cache[i - 1];
            t1 -= 12 * cache[i - 2];
            t1 -=  2 * cache[i - 3];
            t1 +=  3 * cache[i - 4];
            t1 +=  3 * cache[i - 5];
            t1 +=  2 * cache[i - 6];
            cache[i] = t1;
        }
        cache_size = n;
    }

    return cache[n];
}
