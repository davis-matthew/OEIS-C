/*
 * A318406  For n > 4, a(n) = a(n-1) + a(n-2) if n is even and a(n) = 3*a(n-2)
 *          + a(n-4) - a(n-5) if n is odd; a(0) = 0, a(1) = 1, a(2) = 1, a(3)
 *          = 2, and a(4) = 3.
 * https://oeis.org/A318406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - 2*a(n-4) - 2*a(n-6) for n>5, with a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=7
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 40, u64 n <= 78, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;

    /* Ensure the cache can hold index n. */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? (cap << 1) : 16;
        while (newcap <= (size_t)n) newcap <<= 1;
        /* realloc in C++ returns void*, so cast. */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap = newcap;
    }

    /* Initialise the first six terms on first call. */
    if (len == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 2;
        cache[4] = 3;
        cache[5] = 7;
        len    = 6;
    }

    /* Return cached value if already computed. */
    if ((size_t)n < len)
        return cache[(size_t)n];

    /* Compute any missing terms up to index n. */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = 4 * cache[i - 2];
        val -= 2 * cache[i - 4];
        val -= 2 * cache[i - 6];
        cache[i] = val;
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
