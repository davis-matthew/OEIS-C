/*
 * A345448  Number of tilings of a 2 X n rectangle with dominoes and long
 *          L-shaped 4-minoes.
 * https://oeis.org/A345448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 4*a(n-3) + 2*a(n-4) a(0) = 1, a(1) = 1,
 *            a(2) = 2, a(3) = 7
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static backing cache – grows as needed */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    /* small index shortcut – the first four terms */
    if (n < 4) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)7;
        }
    }

    /* initialise cache on first call */
    if (!cache) {
        cap = 4;                       /* enough for the base values   */
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
    }

    /* grow cache if n is beyond current capacity */
    if (n >= cap) {
        idx_t new_cap = cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2]
                 + (seq_t)4 * cache[i-3]
                 + (seq_t)2 * cache[i-4];
    }

    return cache[n];
}
