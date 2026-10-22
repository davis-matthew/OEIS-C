/*
 * A309998  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2 + (x + 529)^2 = y^2.
 * https://oeis.org/A309998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-5) - a(n-10) + 1058 for n >= 11, with initial
 *            values a(1)=0, a(2)=276, a(3)=287, a(4)=740, a(5)=759,
 *            a(6)=1587, a(7)=3059, a(8)=3120, a(9)=5687, a(10)=5796
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 46, u64 n <= 110, u128 n <= 236,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t idx = (size_t)(n - 1);          /* zero‑based index */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise cache with first ten terms on first call */
    if (cache_len == 0) {
        cache_cap = 10;
        cache_len = 10;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)276;
        cache[2] = (seq_t)287;
        cache[3] = (seq_t)740;
        cache[4] = (seq_t)759;
        cache[5] = (seq_t)1587;
        cache[6] = (seq_t)3059;
        cache[7] = (seq_t)3120;
        cache[8] = (seq_t)5687;
        cache[9] = (seq_t)5796;
    }

    /* enlarge cache if needed */
    if (idx >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 10;
        while (new_cap <= idx) {
            size_t doubled = new_cap * 2;
            if (doubled <= new_cap)   /* overflow protection */
                new_cap = idx + 1;
            else
                new_cap = doubled;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 10) continue;  /* already set */
        seq_t val = ((seq_t)6 * cache[i - 5]) - cache[i - 10] + (seq_t)1058;
        cache[i] = val;
    }
    if (cache_len <= idx + 1)
        cache_len = idx + 1;

    return cache[idx];
}
