/*
 * A331444  Number of 2-complete partitions of n with largest part 4.
 * https://oeis.org/A331444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-2*a(n-5)+a(n-8)+a(n-9)-a(n-10) for n>11 *
 *            Initial values: a(0)=0 a(1)=0 a(2)=0 a(3)=0 a(4)=0 a(5)=0 *
 *            a(6)=1 a(7)=2 a(8)=3 a(9)=4 a(10)=6 a(11)=9 * * Implementation
 *            uses a static cache that grows geometrically
 * Range:     u8 n <= 33, u16 n <= 211, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* 12-term lookup for indices 0..11 */
static const seq_t base[12] = {
    0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 9
};

seq_t a(idx_t n)
{
    /* Cached values: indices 0 .. cap-1 */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* Grow cache if needed. The use of size_t ensures the cast
       from idx_t is safe for common platforms. */
    if (cap <= (size_t)n) {
        size_t new_cap = (cap == 0 ? 1 : cap);
        while (new_cap <= (size_t)n) new_cap <<= 1;

        /* realloc may return NULL on failure; abort in that case. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute all terms up to n (recomputation each call is inexpensive). */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < 12) {
            cache[i] = base[i];
        } else {
            cache[i] =
                cache[i-1]
              + cache[i-2]
              + cache[i-8]
              + cache[i-9]
              - (cache[i-5] << 1)   /* 2 * a(i-5) */
              - cache[i-10];
        }
    }

    return cache[n];
}
