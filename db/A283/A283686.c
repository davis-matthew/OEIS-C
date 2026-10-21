/*
 * A283686  Number of nX3 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A283686:
 * a(1) = 8, a(2) = 49, a(3) = 264, a(4) = 1525, a(5) = 8732,
 * a(6) = 49924, a(7) = 285770, a(8) = 1635402, a(9) = 9359104,
 * a(10) = 53561285, a(11) = 306524651, and for n > 10
 *
 * a(n) = 5*a(n-1) + 2*a(n-2) + 14*a(n-3) + 2*a(n-5)
 *        + 24*a(n-9) + 40*a(n-10)
 *        - 9*a(n-4) - 45*a(n-6) - 20*a(n-7) - 6*a(n-8)
 *
 * The function memoises previously computed values so any order
 * of calls is supported.  It is O(n) per new maximum n.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* cache size (index 0 .. cap-1) */

    /* initial values a(1) .. a(10)   (index 0 unused) */
    static const seq_t init[11] = {
        0,          /* 0th index not used */
        8,          /* a(1)  */
        49,         /* a(2)  */
        264,        /* a(3)  */
        1525,       /* a(4)  */
        8732,       /* a(5)  */
        49924,      /* a(6)  */
        285770,     /* a(7)  */
        1635402,    /* a(8)  */
        9359104,    /* a(9)  */
        53561285    /* a(10)*/
    };

    /* initialise cache on first call */
    if (!cache) {
        cap = 11;                                 /* indices 0..10 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;                      /* unused */
        for (idx_t i = 1; i <= 10; ++i)            /* copy initial terms */
            cache[i] = init[i];
    }

    /* return early if the requested term is already computed */
    if (n < (idx_t)cap)
        return cache[n];

    /* extend cache to hold indices up to n */
    size_t new_cap = (size_t)n + 1;    /* need indices 0 .. n */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failed – return 0 as a safe fallback */
        return (seq_t)0;
    cache = tmp;
    cap = new_cap;

    /* compute new terms using the recurrence */
    for (size_t k = 11; k <= (size_t)n; ++k) {
        seq_t term =
            (seq_t)5  * cache[k-1] +
            (seq_t)2  * cache[k-2] +
            (seq_t)14 * cache[k-3] +
            (seq_t)2  * cache[k-5] +
            (seq_t)24 * cache[k-9] +
            (seq_t)40 * cache[k-10];
        term -= (seq_t)9   * cache[k-4];
        term -= (seq_t)45  * cache[k-6];
        term -= (seq_t)20  * cache[k-7];
        term -= (seq_t)6   * cache[k-8];
        cache[k] = term;
    }

    return cache[n];
}
