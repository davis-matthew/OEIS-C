/*
 * A301404  Number of n X 5 0..1 arrays with every element equal to 0, 1, 2,
 *          3, 4 or 5 horizontally, vertically or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A301404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) + 204*a(n-2) + 228*a(n-3) - 1008*a(n-4) -
 *            1008*a(n-5) + 2304*a(n-6) - 192*a(n-7) - 512*a(n-8) for n > 9
 *            Initial values: a(1) = 16 a(2) = 512 a(3) = 15680 a(4) = 483600
 *            a(5) = 14905344 a(6) = 459428416 a(7) = 14160945920 a(8) =
 *            436482419456 a(9) = 13453684678656
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;
    static idx_t    last  = 0;   /* highest index for which cache[i] is valid */

    if (n < 1)           /* undefined for n < 1 – return 0 */
        return 0;

    if (cache == NULL) {
        /* initialise cache with first 9 terms, leaving cache[0] unused */
        cap  = 10;                      /* indices 0..9 inclusive */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;    /* allocation failed */

        cache[0] = 0;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)512;
        cache[3] = (seq_t)15680;
        cache[4] = (seq_t)483600;
        cache[5] = (seq_t)14905344;
        cache[6] = (seq_t)459428416;
        cache[7] = (seq_t)14160945920;
        cache[8] = (seq_t)436482419456;
        cache[9] = (seq_t)13453684678656;
        last   = 9;
    }

    /* Expand cache capacity if required */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap;
        while ((size_t)n + 1 > newcap)
            newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)
            return 0;   /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Return cached value if already computed */
    if (n <= last)
        return cache[(size_t)n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 0;

        val += 24  * cache[(size_t)(i-1)];
        val += 204 * cache[(size_t)(i-2)];
        val += 228 * cache[(size_t)(i-3)];
        val -= 1008 * cache[(size_t)(i-4)];
        val -= 1008 * cache[(size_t)(i-5)];
        val += 2304 * cache[(size_t)(i-6)];
        val -= 192  * cache[(size_t)(i-7)];
        val -= 512  * cache[(size_t)(i-8)];

        cache[(size_t)i] = val;
    }

    last = n;
    return cache[(size_t)n];
}
