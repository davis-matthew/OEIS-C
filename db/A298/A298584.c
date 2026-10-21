/*
 * A298584  Number of nX3 0..1 arrays with every element equal to 0, 3, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A298584: Number of n×3 0..1 arrays with every element equal to 0, 3, 4, 5 or 7 king‑move adjacent elements,
   with upper left element zero.  The sequence satisfies the recurrence
   a(n) = a(n‑1) + 2*a(n‑2) + 2*a(n‑3) - 4*a(n‑4) - 2*a(n‑5)
         - a(n‑6) - a(n‑7) + a(n‑9)   for n > 13,
   with initial values a(1)=1, a(2)=1, a(3)=9, a(4)=5, a(5)=16, a(6)=31,
   a(7)=28, a(8)=87, a(9)=125, a(10)=185, a(11)=418, a(12)=596, a(13)=1073.
   The function is memoised and works for arbitrary order of calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoisation */
    static seq_t *cache     = NULL;   /* 1‑based array; index 0 unused          */
    static idx_t cache_cap  = 0;       /* current allocated capacity (size_t)   */
    static idx_t max_computed = 0;     /* highest index for which cache[i] is valid */

    if (n < 1)
        return 0;   /* behaviour below the offset is undefined */

    /* Ensure storage is large enough to hold indices up to n.          */
    if (cache_cap <= n) {
        size_t new_cap = n > 16 ? (n * 2) : 16;   /* avoid too small allocations */
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure: terminate program (no std::exit for C++). */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values on the first call or if incomplete.       */
    if (max_computed < 13) {
        /* Ensure we have space for indices 1..13. */
        if (cache_cap < 14) {
            if (cache) free(cache);
            cache = (seq_t *)malloc(14 * sizeof(seq_t));
            if (!cache) abort();
            cache_cap = 14;
        }

        /* Assign known initial values (overflows in small seq_t are allowed). */
        cache[1]  = (seq_t)1;
        cache[2]  = (seq_t)1;
        cache[3]  = (seq_t)9;
        cache[4]  = (seq_t)5;
        cache[5]  = (seq_t)16;
        cache[6]  = (seq_t)31;
        cache[7]  = (seq_t)28;
        cache[8]  = (seq_t)87;
        cache[9]  = (seq_t)125;
        cache[10] = (seq_t)185;
        cache[11] = (seq_t)418;
        cache[12] = (seq_t)596;
        cache[13] = (seq_t)1073;
        max_computed = 13;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* For i <= 13 the values are already in cache. */
        if (i > 13) {
            seq_t val = cache[i-1]
                     + (seq_t)2 * cache[i-2]
                     + (seq_t)2 * cache[i-3]
                     - (seq_t)4 * cache[i-4]
                     - (seq_t)2 * cache[i-5]
                     - cache[i-6]
                     - cache[i-7]
                     + cache[i-9];
            cache[i] = val;
        }
        max_computed = i;
    }

    return cache[n];
}
