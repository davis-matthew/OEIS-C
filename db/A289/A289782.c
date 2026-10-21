/*
 * A289782  p-INVERT of the Lucas numbers (A000032), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - 7*a(n-3) + a(n-4) a(0) = 2, a(1) =
 *            9, a(2) = 35, a(3) = 146
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to support arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored (indices 0 .. len-1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise first terms */
    if (cache == NULL)
    {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure: return 0 as fallback */
        cache[0] = 2;  cache[1] = 9;  cache[2] = 35;  cache[3] = 146;
        cache_len = 4;
    }

    /* grow cache if needed */
    size_t idx = (size_t)n;
    if (idx >= cache_cap)
    {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= idx) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* extend cache up to n */
    while (cache_len <= idx)
    {
        if (cache_len >= 4) /* recurrence can be applied */
        {
            seq_t a_nm1 = cache[cache_len - 1];
            seq_t a_nm2 = cache[cache_len - 2];
            seq_t a_nm3 = cache[cache_len - 3];
            seq_t a_nm4 = cache[cache_len - 4];
            cache[cache_len] = 4 * a_nm1 + 2 * a_nm2 - 7 * a_nm3 + a_nm4;
        }
        cache_len++;
    }

    return cache[idx];
}
