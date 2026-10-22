/*
 * A366143  a(n) = a(n-2) + 2*a(n-4) - a(n-10), with a[0..9] = [1, 1, 1, 1, 1,
 *          2, 3, 5, 6, 9].
 * https://oeis.org/A366143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-4) - a(n-10) with initial values a[0..9] =
 *            [1, 1, 1, 1, 1, 2, 3, 5, 6, 9]
 * Range:     u8 n <= 18, u16 n <= 35, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* maximum n we expect (≈10000) + some guard */
    enum { MAX_N = 10009 };

    /* Lazy static cache */
    static seq_t cache[MAX_N + 1];
    static idx_t  last = 9;
    static int    init = 0;

    if (!init) {
        cache[0] = 1; cache[1] = 1; cache[2] = 1; cache[3] = 1; cache[4] = 1;
        cache[5] = 2; cache[6] = 3; cache[7] = 5; cache[8] = 6; cache[9] = 9;
        init = 1;
    }

    if (n <= last)
        return cache[n];

    /* extend cache sequentially up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence valid for i >= 10; for i < 10 references are already defined */
        cache[i] = cache[i - 2] + (seq_t)2 * cache[i - 4] - cache[i - 10];
    }
    last = n;

    return cache[n];
}
