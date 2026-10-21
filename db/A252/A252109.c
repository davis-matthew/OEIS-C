/*
 * A252109  Number of (n+2) X (3+2) 0..3 arrays with every 3 X 3 subblock row
 *          and column sum equal to 0 2 3 6 or 7 and every 3 X 3 diagonal and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) – 8*a(n-6) for n > 12 Initial values (a(1) …
 *            a(12)): 1568, 724, 554, 776, 1258, 1634, 2476, 3938, 5956, 9176,
 *            14532, 22664
 * Range:     u8 none, u16 n <= 14, u32 n <= 38, u64 n <= 86, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t base_vals[13] = {
    0ULL,        /* a(0) unused */
    1568ULL,     /* a(1) */
    724ULL,      /* a(2) */
    554ULL,      /* a(3) */
    776ULL,      /* a(4) */
    1258ULL,     /* a(5) */
    1634ULL,     /* a(6) */
    2476ULL,     /* a(7) */
    3938ULL,     /* a(8) */
    5956ULL,     /* a(9) */
    9176ULL,     /* a(10) */
    14532ULL,    /* a(11) */
    22664ULL     /* a(12) */
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* offset starts at 1 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* highest n we have computed */

    /* first call: allocate space for the basis values (indices 0..12) */
    if (cache == NULL) {
        size_t cap = (n > 12) ? (size_t)n + 1 : 13;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            exit(1);
        for (idx_t i = 0; i <= 12; ++i)
            cache[i] = base_vals[i];
        cache_len = 12;
    }

    /* grow the cache if necessary */
    if (n > cache_len) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > (size_t)cache_len + 1) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                exit(1);
            cache = tmp;
        }
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* recurrence only needed for k >= 13 */
            if (k <= 12)
                continue;
            cache[k] = 6 * cache[k - 3] - 8 * cache[k - 6];
        }
        cache_len = n;
    }

    return cache[n];
}
