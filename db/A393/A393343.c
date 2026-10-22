/*
 * A393343  Number of Cayley permutations of size n avoiding 132 and 213.
 * https://oeis.org/A393343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 10*a(n-3) - 2*a(n-4) (n ≥ 5)
 *            initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=11, a(4)=42
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* cache of computed values */
static seq_t *cache = NULL;      /* dynamic array */
static size_t cache_cap = 0;     /* allocated capacity  (terms) */
static size_t cache_len = 0;     /* number of terms stored (next free index) */

/* allocate / grow the cache as needed (size in terms) */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap <= need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    /* zero the newly allocated tail (not strictly required but keeps values defined) */
    if (new_cap > cache_len)
        memset(new_cache + cache_len, 0, (new_cap - cache_len) * sizeof(seq_t));
    cache       = new_cache;
    cache_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* ensure the cache can hold index n */
    size_t need = (size_t)n + 1;      /* one past the desired index */
    if (need > cache_cap)
        ensure_capacity(need);

    /* compute missing values up to n (caching results) */
    for (size_t k = cache_len; k <= (size_t)n; ++k)
    {
        if (k == 0)
            cache[0] = 1;
        else if (k == 1)
            cache[1] = 1;
        else if (k == 2)
            cache[2] = 3;
        else if (k == 3)
            cache[3] = 11;
        else if (k == 4)
            cache[4] = 42;
        else {
            /* recurrence valid for k ≥ 5 */
            seq_t t1 = 6 * cache[k-1];
            seq_t t2 = 11 * cache[k-2];
            seq_t t3 = 10 * cache[k-3];
            seq_t t4 = 2  * cache[k-4];
            cache[k] = t1 - t2 + t3 - t4;
        }
    }
    cache_len = need;   /* now we have values up to n */
    return cache[n];
}
