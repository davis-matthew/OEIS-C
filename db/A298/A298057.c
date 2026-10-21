/*
 * A298057  Number of n X 2 0..1 arrays with every element equal to 1, 3, 4 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+8*a(n-3)-16*a(n-5), a(1)=1, a(2)=4, a(3)=3,
 *            a(4)=13, a(5)=32
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holds computed values, 1‑based indexing. */
    static seq_t *cache = NULL;      /* array of a(n) values */
    static size_t cap   = 0;         /* allocated capacity (in elements) */
    static idx_t max_n = 0;          /* highest n computed so far */

    /* handle invalid (non‑positive) indices gracefully */
    if (n == 0) return 0;

    /* initialize cache on first use */
    if (cache == NULL) {
        cap = 6;                                    /* indices 0..5 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;                /* allocation error */
        cache[0] = 0;                               /* unused */
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 3;
        cache[4] = 13;
        cache[5] = 32;
        max_n = 5;
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;            /* allocation error */
        cache = new_cache;
        cap = new_cap;
    }

    /* compute values up to n if not already computed */
    if (n > max_n) {
        idx_t i;
        for (i = max_n + 1; i <= n; ++i) {
            if (i <= 5) {
                /* base cases already set during initialization */
                continue;
            }
            /* recurrence: a(i) = a(i-1)+a(i-2)+8*a(i-3)-16*a(i-5) */
            seq_t val = cache[i - 1] + cache[i - 2] + (seq_t)8 * cache[i - 3];
            val -= (seq_t)16 * cache[i - 5];
            cache[i] = val;
        }
        max_n = n;
    }

    return cache[n];
}
