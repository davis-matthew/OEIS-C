/*
 * A291243  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - 3 S + S^2.
 * https://oeis.org/A291243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 3*a(n-3) - a(n-4) Initial values:
 *            a(0) = 3, a(1) = 8, a(2) = 24, a(3) = 71, a(4) = 210
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t sz   = 0;     /* number of entries currently stored */

    /* Initialise with the first five terms */
    if (sz == 0) {
        grow_seq(&cache, &cap, 5);
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)24;
        cache[3] = (seq_t)71;
        cache[4] = (seq_t)210;
        sz = 5;
    }

    /* If requested index is beyond what we have, extend the cache */
    if ((size_t)n >= sz) {
        grow_seq(&cache, &cap, (size_t)n + 1);
        for (size_t i = sz; i <= (size_t)n; ++i) {
            /* recurrence: a(i) = 3*a(i-1) + a(i-2) - 3*a(i-3) - a(i-4) */
            seq_t val = 3 * cache[i - 1]
                        + cache[i - 2]
                        - 3 * cache[i - 3]
                        - cache[i - 4];
            cache[i] = val;
        }
        sz = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
