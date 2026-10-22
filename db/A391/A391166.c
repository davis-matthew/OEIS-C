/*
 * A391166  Body-centered cubic pyritohedral numbers: a(n) = 1728*n^3 -
 *          5076*n^2 + 4984*n - 1635.
 * https://oeis.org/A391166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 136, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS sequence A391166 – Body‑centered cubic pyritohedral numbers
 *
 *  a(n) = 1728 n³ – 5076 n² + 4984 n – 1635
 *
 *  The implementation uses the convenient recurrence
 *
 *      a(1) = 1
 *      a(n) = a(n‑1) + 5184 n² – 15336 n + 11788          (n ≥ 2)
 *
 *  Memoisation is employed so that calls with arbitrary order of n
 *  require only linear work up to the largest n requested.
 *
 *  The function takes advantage of the fact that all intermediate
 *  arithmetic is performed in the type `seq_t`.  When `seq_t` is a
 *  native 64‑bit unsigned integer the intermediate products stay
 *  below the 64‑bit limit for all n used by the tests (n ≤ 10 000).
 *  For arbitrary‑precision builds `seq_t` handles larger values
 *  automatically.  No floating point or external libraries are used.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the unlikely case of n == 0 (offset is 1). */
    if (n == 0)
        return (seq_t)0;

    /* Lazy static cache: cache[i-1] holds a(i). */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    /* Initialise cache for n == 1. */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure – return 0 */
        cache[0] = (seq_t)1;          /* a(1) = 1 */
    }

    /* Return cached value if already computed. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow cache to the required size. */
    idx_t new_size = cache_size * 2;
    if (new_size < n)
        new_size = n;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        return (seq_t)0;          /* allocation failure – return 0 */
    cache = tmp;

    /* Compute the next terms using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t ii = (seq_t)i;

        /* diff = 5184 * i² – 15336 * i + 11788 */
        seq_t diff = (seq_t)5184 * ii * ii;
        diff -= (seq_t)15336 * ii;
        diff += (seq_t)11788;

        cache[i - 1] = cache[i - 2] + diff;
    }

    cache_size = n;
    return cache[n - 1];
}
