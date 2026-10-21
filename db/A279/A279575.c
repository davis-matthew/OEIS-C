/*
 * A279575  Number of n X 3 0..2 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 44*a(n-1) - 556*a(n-2) + 1616*a(n-3) - 2000*a(n-4) +
 *            1152*a(n-5) - 256*a(n-6) (n>7) Initial terms: a(1)=2, a(2)=36,
 *            a(3)=944, a(4)=23072, a(5)=547168, a(6)=12701248, a(7)=290067328
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n == 0)
        return 0;  /* undefined for n<1, return 0 */

    /* Ensure cache is initialized with at least the first 7 terms */
    if (cache == NULL) {
        cache_cap = n > 16 ? n : 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure */

        cache[0] = 2;
        cache[1] = 36;
        cache[2] = 944;
        cache[3] = 23072;
        cache[4] = 547168;
        cache[5] = 12701248;
        cache[6] = 290067328;
        cache_len = (cache_cap < 7) ? cache_cap : 7;
    }

    if ((size_t)n <= cache_len)
        return cache[n-1];

    /* Grow cache if needed */
    while ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t term =
            ( 44 * cache[i-1]) -
            (556 * cache[i-2]) +
            (1616 * cache[i-3]) -
            (2000 * cache[i-4]) +
            (1152 * cache[i-5]) -
            ( 256 * cache[i-6]);
        cache[i] = term;
    }
    cache_len = n;
    return cache[n-1];
}
