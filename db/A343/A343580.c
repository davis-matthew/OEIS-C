/*
 * A343580  a(n) = abs(A021009(n, floor(n/2))).
 * https://oeis.org/A343580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/2)) * FallingFactorial(n, n -
 *            floor(n/2))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache   = NULL; /* array of seq_t values */
    static size_t cache_cap = 0;  /* allocated capacity of cache */
    static idx_t  cache_size= 0;  /* number of values actually stored  */

    /* Expand cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failed – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n < cache_size) return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if ((i & 1ULL) == 0) {          /* even index */
            cache[i] = cache[i-1] * (seq_t)4;
        } else {                               /* odd index */
            idx_t m = (i - 1) >> 1;            /* floor((i-1)/2) */
            seq_t val = cache[i-1];
            val /= (seq_t)(m + 1);             /* exact division */
            seq_t sq = (seq_t)i * (seq_t)i;
            val *= sq;
            cache[i] = val;
        }
    }
    cache_size = n + 1;   /* now cache contains 0 .. n */
    return cache[n];
}
