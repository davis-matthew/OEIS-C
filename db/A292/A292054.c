/*
 * A292054  Wiener index of the n X n knight graph.
 * https://oeis.org/A292054
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 3*a(n-3) - a(n-5) + 2*a(n-6) +
 *            2*a(n-7) - a(n-8) - 3*a(n-10) + a(n-11) + 2*a(n-12) - a(n-13)
 *            (for n > 17) Initial values: a(4)=288, a(5)=708, a(6)=1580,
 *            a(7)=3144, a(8)=5804, a(9)=9996, a(10)=16388, a(11)=25660,
 *            a(12)=38808, a(13)=56808, a(14)=81048, a(15)=112856, a(16)...
 * Range:     u8 none, u16 n <= 13, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 4 – return 0 as a guard. */
    if (n < 4) return 0;

    /* Direct lookup for the first 14 terms. */
    if (n <= 17) {
        static const seq_t init_vals[14] = {
            288, 708, 1580, 3144, 5804, 9996,
            16388, 25660, 38808, 56808, 81048,
            112856, 154080, 206448
        };
        return init_vals[n - 4];
    }

    /* Static cache for memoisation: a(4) stored at cache[0], … */
    static seq_t *cache = NULL;
    static size_t  capacity = 0;          /* allocated size */
    static size_t  stored   = 0;          /* number of values stored */
    static idx_t   last_n   = 0;          /* largest n computed so far */

    /* Initialise cache with the first 14 values. */
    if (cache == NULL) {
        capacity = 18; /* enough for n = 17; will grow as needed */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) abort();
        static const seq_t init_vals[14] = {
            288, 708, 1580, 3144, 5804, 9996,
            16388, 25660, 38808, 56808, 81048,
            112856, 154080, 206448
        };
        for (size_t i = 0; i < 14; ++i) cache[i] = init_vals[i];
        stored = 14;
        last_n = 17;
    }

    /* Extend cache up to n, computing missing terms. */
    while (last_n < n) {
        idx_t next_n = last_n + 1;
        /* Ensure capacity. */
        if (stored == capacity) {
            capacity <<= 1;           /* double the capacity */
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * capacity);
            if (!tmp) abort();
            cache = tmp;
        }

        /* Recurrence calculation for next_n. */
        seq_t val = 0;
        val += (seq_t)2 * cache[(next_n - 1) - 4];
        val +=            cache[(next_n - 2) - 4];
        val -= (seq_t)3 * cache[(next_n - 3) - 4];
        val -=            cache[(next_n - 5) - 4];
        val += (seq_t)2 * cache[(next_n - 6) - 4];
        val += (seq_t)2 * cache[(next_n - 7) - 4];
        val -=            cache[(next_n - 8) - 4];
        val -= (seq_t)3 * cache[(next_n -10) - 4];
        val +=            cache[(next_n -11) - 4];
        val += (seq_t)2 * cache[(next_n -12) - 4];
        val -=            cache[(next_n -13) - 4];

        cache[stored++] = val;
        last_n = next_n;
    }

    return cache[n - 4];
}
