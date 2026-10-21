/*
 * A252857  Number of (n+2) X (4+2) 0..2 arrays with every consecutive three
 *          elements in every row, column, diagonal and antidiagonal having
 *          exactly two distinct values, and new values 0 upwards introduced
 *          in row major order.
 * https://oeis.org/A252857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 4*a(n-3) + 6*a(n-4) - a(n-5) - 5*a(n-6) -
 *            5*a(n-7) + 2*a(n-9) - a(n-10) for n>12
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=1719, a(2)=1296, a(3)=1962, a(4)=3582, a(5)=7002, a(6)=13590, a(7)=26766, a(8)=52650, a(9)=104796, a(10)=207414, a(11)=410922, a(12)=815634 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;  /* cache[0..max_n] (0 unused) */
    static size_t cache_size = 0;     /* allocated size */
    static idx_t  max_n      = 0;     /* highest index computed */

    /* ensure the cache can hold index n */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;          /* 1‑based indexing */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                      /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise dummy element if needed */
    if (max_n == 0 && cache_size > 0) {
        cache[0] = 0;
    }

    /* if already computed, return it */
    if (n <= max_n)
        return cache[n];

    /* initial constants for n = 1..12 */
    static const seq_t init[13] = {
        0ULL,                /* unused for 0 */
        1719ULL, 1296ULL, 1962ULL, 3582ULL,
        7002ULL, 13590ULL, 26766ULL, 52650ULL,
        104796ULL, 207414ULL, 410922ULL, 815634ULL
    };

    /* compute terms up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[i] = init[i];
        } else {
            seq_t val = cache[i - 2];
            val += cache[i - 3] * 4;
            val += cache[i - 4] * 6;
            val += cache[i - 9] * 2;
            val -= cache[i - 5];
            val -= cache[i - 6] * 5;
            val -= cache[i - 7] * 5;
            val -= cache[i - 10];
            cache[i] = val;
        }
    }

    max_n = n;
    return cache[n];
}
