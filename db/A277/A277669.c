/*
 * A277669  Number of n-length words over a 6-ary alphabet {a_1,a_2,...,a_6}
 *          avoiding consecutive letters a_i, a_{i+1}.
 * https://oeis.org/A277669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): number of length‑n 6‑ary words avoiding consecutive
 * letters a_i, a_{i+1}.  
 * Recurrence:
 *   f_k[1] = a(k-1)
 *   f_k[i] = a(k-1) - f_{k-1}[i-1]   (2 ≤ i ≤ 6)
 *   a(k) = Σ_i f_k[i]
 *
 * The implementation uses linear‑time dynamic programming with memoisation.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static bool initialized = false;
    static seq_t *cache = NULL;      /* a[0] .. a[last] */
    static size_t cache_capacity = 0;
    static size_t cache_len = 0;      /* number of stored values */
    static seq_t prev[6];            /* f_{k-1}[i] for the last computed k-1 */
    static seq_t sum_prev;           /* a(k-1) for the last computed k-1 */

    /* initialise once */
    if (!initialized) {
        cache_capacity = 8;          /* arbitrary initial capacity */
        cache = (seq_t*)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)1;        /* a(0) */
        cache[1] = (seq_t)6;        /* a(1) */

        for (int i = 0; i < 6; ++i) prev[i] = (seq_t)1;  /* f_1[i] */
        sum_prev = (seq_t)6;                      /* a(1) */

        cache_len = 2;              /* we have stored a(0) and a(1) */
        initialized = true;
    }

    /* return cached value if already available */
    if (n < cache_len)
        return cache[n];

    /* grow cache if necessary and compute missing values up to n */
    while (cache_len <= n) {
        /* ensure capacity */
        if (cache_len >= cache_capacity) {
            size_t new_capacity = cache_capacity * 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_capacity = new_capacity;
        }

        seq_t curr[6];
        curr[0] = sum_prev;                    /* f_k[1] */
        for (int i = 1; i < 6; ++i)
            curr[i] = sum_prev - prev[i-1];    /* f_k[i] (i ≥ 2) */

        seq_t sum_curr = (seq_t)0;
        for (int i = 0; i < 6; ++i)
            sum_curr += curr[i];

        cache[cache_len] = sum_curr;

        for (int i = 0; i < 6; ++i)
            prev[i] = curr[i];
        sum_prev = sum_curr;
        ++cache_len;
    }

    return cache[n];
}
