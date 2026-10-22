/*
 * A335711  The number of free polyominoes of width 2 and height n.
 * https://oeis.org/A335711
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + 3*a(n-4)
 *                - a(n-5) + a(n-6) + a(n-7)   for n ≥ 9
 * Initial values: a(2)=2, a(3)=6, a(4)=12, a(5)=30, a(6)=65,
 *                 a(7)=158, a(8)=362
 * Undefined for n<2 (return 0). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Simple static memoisation cache. */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity   */
    static idx_t cache_max   = 1;   /* largest n cached     */

    /* ensure capacity for index n */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure   */
        /* zero‐fill new slots */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;

        cache = tmp;
        cache_cap = new_cap;
    }

    /* Integer constants used below are implicitly convertible to seq_t. */
    static const seq_t init[9] = {
        0,                      /* 0 – unused       */
        0,                      /* 1 – unused       */
        2,                      /* 2   */
        6,                      /* 3   */
        12,                     /* 4   */
        30,                     /* 5   */
        65,                     /* 6   */
        158,                    /* 7   */
        362                     /* 8   */
    };

    /* compute all missing values up to n */
    if (cache_max < n) {
        idx_t start = cache_max + 1;
        if (start < 2) start = 2;  /* offset is 2 */

        for (idx_t i = start; i <= n; ++i) {
            if (i <= 8) {
                cache[i] = init[i];
            } else {
                /* recurrence for n >= 9 */
                seq_t val =
                    3 * cache[i-1] + cache[i-2]
                    - 7 * cache[i-3] + 3 * cache[i-4]
                    -   cache[i-5] +   cache[i-6]
                    +   cache[i-7];
                cache[i] = val;
            }
        }
        cache_max = n;
    }

    if (n < 2) return 0;   /* undefined below the offset */
    return cache[n];
}
