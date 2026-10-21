/*
 * A293846  Numbers such that k is the altitude of a Heronian triangle with
 *          sides m-13, m, m+13.
 * https://oeis.org/A293846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 44, u64 n <= 94, u128 n <= 195,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A293846: a(n) = 4*a(n-3) - a(n-6), a(0)=9, a(1)=24, a(2)=39,
   a(3)=60, a(4)=105, a(5)=156 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static const seq_t init[6] = {9, 24, 39, 60, 105, 156};

    /* handle the first six terms directly */
    if (n < 6)
        return init[(size_t)n];

    size_t idx = (size_t)n;

    /* initialise cache on first use */
    if (cache == NULL) {
        cap = 6;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        if (cache == NULL) return 0;              /* out of memory – fall back */
        for (size_t i = 0; i < 6; ++i) cache[i] = init[i];
        len = 6;
    }
    /* grow cache if needed */
    if (idx >= cap) {
        size_t new_cap = cap;
        while (idx >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return 0;                /* out of memory – fall back */
        cache = tmp;
        cap   = new_cap;
    }
    /* compute missing entries */
    for (size_t i = len; i <= idx; ++i) {
        /* recurrence: a[i] = 4*a[i-3] - a[i-6] */
        seq_t val = 4 * cache[i - 3] - cache[i - 6];
        cache[i] = val;
    }
    len = idx + 1;
    return cache[idx];
}
