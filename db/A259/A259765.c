/*
 * A259765  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 or 00000101.
 * https://oeis.org/A259765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + 6*a(n-4) + 2*a(n-5) - a(n-6) +
 *            2*a(n-7) - a(n-8) for n > 9 Initial values: a(1)=32, a(2)=49,
 *            a(3)=104, a(4)=201, a(5)=376, a(6)=745, a(7)=1494, a(8)=2897,
 *            a(9)=5610
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 62, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const seq_t init_terms[9] = {
    32ULL, 49ULL, 104ULL, 201ULL, 376ULL,
    745ULL, 1494ULL, 2897ULL, 5610ULL
};

static seq_t *cache = NULL;          /* memoization array */
static idx_t cache_size = 0;         /* number of elements already stored */
static idx_t cache_cap   = 0;         /* allocated capacity */

seq_t a(idx_t n)
{
    if (n <= 9)          /* base cases */
        return init_terms[n - 1];

    /* initialise or grow the cache to hold values up to n */
    if (cache_size == 0) {
        cache_cap = n > 9 ? n : 9;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < 9; ++i) cache[i] = init_terms[i];
        cache_size = 9;
    } else if (cache_cap < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = n;
        /* cache_size is unchanged – already computed values remain */
    }

    /* compute missing terms from cache_size up to n-1 */
    for (idx_t i = cache_size; i < n; ++i) {
        /* i corresponds to index of a(i+1) because cache[0] = a(1) */
        seq_t val = cache[i - 2];          /* a(i-1) = a((i+1)-2) */
        val += cache[i - 3] << 1;          /* 2*a(i-2) */
        val += cache[i - 4] * 6;           /* 6*a(i-3) */
        val += cache[i - 5] << 1;          /* 2*a(i-4) */
        val -= cache[i - 6];               /* -a(i-5) */
        val += cache[i - 7] << 1;          /* 2*a(i-6) */
        val -= cache[i - 8];               /* -a(i-7) */
        cache[i] = val;
    }
    cache_size = n;
    return cache[n - 1];
}
