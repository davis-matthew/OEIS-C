/*
 * A252251  Number of (n+2) X (5+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum not equal to 0 3 5 6 or 7 and every 3 X 3 diagonal
 *          and antidiagonal sum equal to 0 3 5 6 or 7.
 * https://oeis.org/A252251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 25, u32 n <= 59, u64 n <= 128, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A252251
 * Empirical recurrence (for n > 9):
 *     a(n) = a(n-1) + 3*a(n-3) - 3*a(n-4) - a(n-6) + a(n-7)
 * Initial values:
 *     a(1) = 463   a(2) = 302   a(3) = 279   a(4) = 297
 *     a(5) = 327   a(6) = 366   a(7) = 414   a(8) = 492
 *     a(9) = 594
 */

#include <stdlib.h>    /* abort, realloc, size_t */

static seq_t *cache = NULL;          /* dynamic array of computed values */
static size_t cache_capacity = 0;    /* allocated size of cache   */
static idx_t  cache_last = 0;        /* largest n that has been
                                       computed and stored */



/* Ensure the cache has enough space for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* n is unsigned; convert to size_t for safety */
    size_t needed = (size_t)(n + 1);
    if (needed <= cache_capacity)
        return;

    /* allocate new buffer */
    void *buf = realloc(cache, needed * sizeof(seq_t));
    if (!buf)
        abort();                    /* out of memory */

    cache = (seq_t*)buf;
    cache_capacity = needed;
}

/* Compute A252251(a) for the given n */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* below offset; behaviour undefined */

    /* Array of the first nine terms (index 0 unused) */
    static const seq_t init[10] = {
        0,          /* dummy for a(0) */
        463, 302, 279, 297, 327, 366, 414, 492, 594
    };

    /* Initialise cache on first call */
    if (cache_last == 0) {
        ensure_cache(9);              /* need space for indices 0..9   */
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init[i];
        cache_last = 9;               /* last computed index            */
    }

    /* Return early for the already known initial values */
    if (n <= cache_last && n <= 9)
        return cache[n];

    /* Ensure there is enough room for n */
    ensure_cache(n);

    /* Compute new terms up to n */
    /* Start from max(cache_last+1, 10) because recurrence needs terms
       with indices >=1 and <=n-1, n-3, n-4, n-6, n-7.  For n <=9
       recurrence is not used. */
    idx_t start = (cache_last + 1);
    if (start < 10)
        start = 10;

    for (idx_t i = start; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + 3*a(i-3) - 3*a(i-4) - a(i-6) + a(i-7) */
        seq_t val = cache[i - 1];
        val += cache[i - 3] * 3;
        val -= cache[i - 4] * 3;
        val -= cache[i - 6];
        val += cache[i - 7];
        cache[i] = val;
    }

    cache_last = n;
    return cache[n];
}
