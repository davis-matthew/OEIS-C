/*
 * A261258  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 00100101 or 01010101.
 * https://oeis.org/A261258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-4) + a(n-6) for n > 7 Initial
 *            values: a(1)=36, a(2)=41, a(3)=91, a(4)=166, a(5)=321, a(6)=586,
 *            a(7)=1123. For n <= 0 the function returns 0
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 31, u64 n <= 66, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for calloc, realloc */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* undefined for n < 1 */

    /* Static cache of computed values */
#ifdef __cplusplus
    static seq_t *cache = nullptr;
    static idx_t cap   = 0;
    if (cap == 0) {
        cap   = 8;                         /* indices 0..7 inclusive   */
        cache = new seq_t[cap];             /* default‑construct all   */
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)41;
        cache[3] = (seq_t)91;
        cache[4] = (seq_t)166;
        cache[5] = (seq_t)321;
        cache[6] = (seq_t)586;
        cache[7] = (seq_t)1123;
    }

    if (n + (idx_t)1 > cap) {
        idx_t new_cap = n + (idx_t)1;
        seq_t *tmp = new seq_t[new_cap];
        for (idx_t i = 0; i < cap; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
        cap = new_cap;
    }
#else
    static seq_t *cache = NULL;
    static idx_t cap   = 0;
    if (cap == 0) {
        cap   = 8;                          /* indices 0..7 inclusive   */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)41;
        cache[3] = (seq_t)91;
        cache[4] = (seq_t)166;
        cache[5] = (seq_t)321;
        cache[6] = (seq_t)586;
        cache[7] = (seq_t)1123;
    }

    if (n + (idx_t)1 > cap) {
        idx_t new_cap = n + (idx_t)1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }
#endif

    if (n <= (idx_t)7) return cache[n];

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)8; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + (seq_t)2 * cache[i-4] + cache[i-6];
    }
    return cache[n];
}
