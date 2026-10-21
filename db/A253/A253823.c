/*
 * A253823  Octagonal numbers (A000567) which are also centered triangular
 *          numbers (A005448).
 * https://oeis.org/A253823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+1331714*a(n-2)-1331714*a(n-3)-a(n-4)+a(n-5)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1, a(2)=97921, a(3)=1039585, a(4)=130402572385, a(5)=1384429704481 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values a(1) … a(k) stored at cache[i-1] */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms currently stored */
    static size_t cache_cap = 0;     /* allocated capacity of the array      */

    /* Guard against the (unlikely) offset of 0 – the sequence is defined for n≥1 */
    if (n < 1) return 0;

    /* ensure the array is large enough for all required terms */
    if (n > cache_cap)
    {
        size_t needed = (size_t)n;
        size_t new_cap = cache_cap ? (cache_cap << 1) : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure – return 0 (undefined) */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* fill the first five terms if they are not present yet */
    if (cache_len < 5)
    {
        cache[0] = (seq_t)  1ULL;
        cache[1] = (seq_t) 97921ULL;
        cache[2] = (seq_t)1039585ULL;
        cache[3] = (seq_t)130402572385ULL;
        cache[4] = (seq_t)1384429704481ULL;
        cache_len = 5;
    }

    /* compute further terms up to n (recurrence valid for i ≥ 6) */
    for (idx_t i = (cache_len < 6 ? 6 : cache_len + 1); i <= n; ++i)
    {
        /* cache indices: a(i‑1) => cache[i-2], a(i‑2) => cache[i-3], … */
        cache[i-1] = cache[i-2]
                   + (seq_t)1331714 * cache[i-3]
                   - (seq_t)1331714 * cache[i-4]
                   - cache[i-5]
                   + cache[i-6];
    }

    cache_len = n;          /* we now have a(1) … a(n) stored */
    return cache[n-1];      /* cache index 0 corresponds to a(1) */
}
