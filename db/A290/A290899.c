/*
 * A290899  p-INVERT of the positive integers, where p(S) = 1 - S^2 - S^4.
 * https://oeis.org/A290899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 27*a(n-2) + 52*a(n-3) - 63*a(n-4) + 52*a(n-5)
 *            - 27*a(n-6) + 8*a(n-7) - a(n-8), a(0)=0, a(1)=1, a(2)=4,
 *            a(3)=12, a(4)=36, a(5)=110, a(6)=332, a(7)=983
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Returns the nth term of A290899. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t sz   = 0;   /* number of elements already stored */
    static size_t cap  = 0;   /* current capacity of the cache */

    /* Convert the unsigned index to size_t for array access. */
    size_t idx = (size_t)n;

    /* Grow the cache if needed. */
    if (idx >= cap)
    {
        size_t new_cap = (cap ? cap * 2 : 8);
        if (new_cap <= idx) new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure – terminate */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first eight terms if they are not yet stored. */
    if (sz < 8)
    {
        static const unsigned long long base[8] = {
            0ULL, 1ULL, 4ULL, 12ULL, 36ULL, 110ULL, 332ULL, 983ULL
        };
        size_t need = 8;
        if (need > idx + 1) need = idx + 1;
        for (size_t i = sz; i < need; ++i)
            cache[i] = (seq_t)base[i];
        sz = need;
    }

    /* Compute any missing terms up to idx using the recurrence. */
    for (size_t i = sz; i <= idx; ++i)
    {
        cache[i] = 8  * cache[i - 1]
                 - 27 * cache[i - 2]
                 + 52 * cache[i - 3]
                 - 63 * cache[i - 4]
                 + 52 * cache[i - 5]
                 - 27 * cache[i - 6]
                 + 8  * cache[i - 7]
                 -   cache[i - 8];
    }

    sz = idx + 1;
    return cache[idx];
}
