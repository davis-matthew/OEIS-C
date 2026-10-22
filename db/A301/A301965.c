/*
 * A301965  Number of 3Xn 0..1 arrays with every element equal to 0, 1 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-4) for n > 6 with initial values a(1)=4,
 *            a(2)=13, a(3)=16, a(4)=25, a(5)=41, a(6)=74, a(7)=132
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size of cache */
    static size_t len = 0;   /* highest index stored (0 if none) */
    static bool init = false;

    if (!init) {
        /* Allocate space for indices 1..7 (index 0 unused). */
        cap = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;   /* allocation failure – return 0 as a safe fallback */

        cache[1] = 4;
        cache[2] = 13;
        cache[3] = 16;
        cache[4] = 25;
        cache[5] = 41;
        cache[6] = 74;
        cache[7] = 132;
        len = 7;
        init = true;
    }

    if (n <= len) return cache[n];

    /* Expand cache if we need more space. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure – return 0 as safe fallback */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = len + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] - cache[i - 4];
    }
    len = n;
    return cache[n];
}
