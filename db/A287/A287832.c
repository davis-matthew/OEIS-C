/*
 * A287832  Number of words of length n over the alphabet {0,1,...,10} such
 *          that no two consecutive terms have distance 1.
 * https://oeis.org/A287832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=11,a(2)=101,a(3)=929,a(4)=8545,a(5)=78599,a(6)=722973,
 *            a(n)=11*a(n-1)-14*a(n-2)-28*a(n-3)+39*a(n-4)+9*a(n-5)-10*a(n-6)
 *            for n>6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised terms */
static size_t cache_len = 0;     /* current cached size */

seq_t a(idx_t n)
{
    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache_len = 7; /* values for 0 .. 6 */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;     /* allocation failure */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)101;
        cache[3] = (seq_t)929;
        cache[4] = (seq_t)8545;
        cache[5] = (seq_t)78599;
        cache[6] = (seq_t)722973;
    }

    /* Return cached value if already computed. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to hold up to index n. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL)
        return (seq_t)0;                 /* allocation failure */

    cache = tmp;
    size_t old_len = cache_len;
    cache_len = new_len;

    /* Compute new terms using the recurrence. */
    for (size_t i = old_len; i < new_len; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        seq_t a5 = cache[i - 5];
        seq_t a6 = cache[i - 6];

        /* All operations are performed in seq_t, which is either a
           standard unsigned integer type or an arbitrary‑precision
           integer class, so overflows are handled correctly for the
           corresponding build. */
        seq_t val = a1 * (seq_t)11;
        val -= a2 * (seq_t)14;
        val -= a3 * (seq_t)28;
        val += a4 * (seq_t)39;
        val += a5 * (seq_t)9;
        val -= a6 * (seq_t)10;

        cache[i] = val;
    }

    return cache[(size_t)n];
}
