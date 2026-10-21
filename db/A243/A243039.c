/*
 * A243039  Number of length n+3 0..3 arrays with no four elements in a row
 *          with pattern abab (with a!=b) and new values 0..3 introduced in
 *          0..3 order.
 * https://oeis.org/A243039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A243039
   Recurrence:  a(n) = 5*a(n-1) - 3*a(n-2) - 5*a(n-3) - 3*a(n-4) 
                  + 3*a(n-5) + 3*a(n-6) + a(n-7)
   Initial values (n ≥ 1):
   a(1)=14, a(2)=46, a(3)=163, a(4)=602, a(5)=2271,
   a(6)=8655, a(7)=33148
*/

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n < 1)          /* offset 1: values only for n ≥ 1 */
        return (seq_t)0;

    /* 1‑based cache of computed terms */
    static seq_t *cache      = NULL;   /* array[0..] (ignore index 0) */
    static size_t cache_cap  = 0;      /* allocated capacity (≥ n+1) */
    static size_t cache_max  = 0;      /* highest n computed */

    /* First call: allocate and initialize the first 7 terms */
    if (cache == NULL) {
        cache_cap = 8;                /* allocate indices 0..7 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;  /* allocation failed */

        cache[0] = (seq_t)0;          /* unused but present */
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)46;
        cache[3] = (seq_t)163;
        cache[4] = (seq_t)602;
        cache[5] = (seq_t)2271;
        cache[6] = (seq_t)8655;
        cache[7] = (seq_t)33148;
        cache_max = 7;
    }

    /* If requested n is beyond cached maximum, extend and compute */
    if ((size_t)n > cache_max) {
        size_t need_cap = (size_t)n + 1;  /* include index 0 */
        if (need_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * need_cap);
            if (!tmp) return (seq_t)0;  /* allocation failed */
            cache = tmp;
            cache_cap = need_cap;
        }
        /* Compute terms from cache_max+1 up to n */
        size_t i_start = cache_max + 1;
        for (size_t i = i_start; i <= (size_t)n; ++i) {
            seq_t t = 5 * cache[i - 1];
            t -= 3 * cache[i - 2];
            t -= 5 * cache[i - 3];
            t -= 3 * cache[i - 4];
            t += 3 * cache[i - 5];
            t += 3 * cache[i - 6];
            t +=  cache[i - 7];
            cache[i] = t;
        }
        cache_max = (size_t)n;
    }

    return cache[(size_t)n];
}
