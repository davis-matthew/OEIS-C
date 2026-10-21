/*
 * A290996  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - S - S^4.
 * https://oeis.org/A290996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 9*a(n-2) + 7*a(n-3) - a(n-4) for n>=4; a(0)=1,
 *            a(1)=2, a(2)=4, a(3)=9
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t len = 0;
    static size_t cap = 0;

    /* handle first four terms directly */
    if (n < 4) {
        switch ((int)n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)4;
            case 3: return (seq_t)9;
        }
    }

    /* initialise the cache on first use */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)9;
        len   = 4;
    }

    /* grow the table if necessary and fill new entries */
    if ((size_t)n >= len) {
        size_t need = (size_t)n + 1;
        if (need > cap) {
            size_t newcap = cap;
            while (newcap < need) newcap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* out of memory */
            cache = tmp;
            cap = newcap;
        }
        for (size_t i = len; i < need; ++i) {
            cache[i] =
                (seq_t)5 * cache[i-1]
              - (seq_t)9 * cache[i-2]
              + (seq_t)7 * cache[i-3]
              -       cache[i-4];
        }
        len = need;
    }

    return cache[(size_t)n];
}
