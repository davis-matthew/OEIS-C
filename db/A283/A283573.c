/*
 * A283573  Number of 2Xn 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (empirical recurrence):
 *     a(n) = 2*a(n-1) +5*a(n-2) +2*a(n-3)
 *            -17*a(n-4) -24*a(n-5) -16*a(n-6)
 * Initial values:
 *     a(1) = 0, a(2) = 4, a(3) = 26, a(4) = 72,
 *     a(5) = 282, a(6) = 908
 *
 * The function uses a simple memoised recurrence computation, storing
 * previously computed terms in a static array.  It works for any
 * unsigned seq_t type that can hold the intermediate values.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t cached_size = 0;        /* highest n for which cache is valid */

    if (n == 0)
        return 0;                        /* undefined for n=0, but safe return */

    /* initialise cache and first six terms on first call */
    if (cache == NULL)
    {
        cached_size = 6;
        cache = (seq_t *)malloc((cached_size + 1) * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory, unlikely */

        /* index 0 is unused; terms start at 1 */
        static const seq_t init[7] = {0, 0, 4, 26, 72, 282, 908};
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init[i];
    }

    /* if request already in cache, return immediately */
    if (n <= cached_size)
        return cache[n];

    /* extend the cache to accommodate n */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) return 0;                  /* out of memory */
    cache = tmp;

    /* fill any new entries using the recurrence */
    for (idx_t i = cached_size + 1; i <= n; ++i)
    {
        seq_t tpos = 2 * cache[i - 1] + 5 * cache[i - 2] + 2 * cache[i - 3];
        seq_t tneg = 17 * cache[i - 4] + 24 * cache[i - 5] + 16 * cache[i - 6];
        cache[i] = tpos - tneg;
    }

    cached_size = n;
    return cache[n];
}
