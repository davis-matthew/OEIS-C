/*
 * A263200  Number of perfect matchings on a Möbius strip of width 3 and
 *          length 2n.
 * https://oeis.org/A263200
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) with a(2)=28, a(3)=104
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max = 1;

    if (n < 2) return (seq_t)0;

    if (!cache) {
        cap = 4;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)28;
        cache[3] = (seq_t)104;
        max = 3;
    }

    if ((size_t)cap <= (size_t)n) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = newcap;
    }

    if (n > max) {
        for (idx_t i = max + 1; i <= n; ++i) {
            cache[i] = (seq_t)4 * cache[i - 1] - cache[i - 2];
        }
        max = n;
    }
    return cache[n];
}
