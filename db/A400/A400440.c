/*
 * A400440  G.f. A(x) satisfies A(x) = (1 + x*A(x^4))^2.
 * https://oeis.org/A400440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; * for n ≥ 0: * a(4*n+1) = 2*a(n); * a(4*n+2) =
 *            Σ_{k=0..n} a(k)*a(n-k); * a(4*n+3) = a(4*n+4) = 0
 * Range:     u8 n <= 125, u16 n <= 609, u32 n <= 3217, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache to allow O(n) computation with any call order. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;    /* allocated size of cache array   */
static size_t cache_limit = 0;  /* highest index that has been computed */

seq_t a(idx_t n)
{
    size_t nn = (size_t)n;

    /* initialise cache for n = 0 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure */
        cache[0] = 1;                  /* a(0) */
        cache_limit = 0;
    }

    /* grow cache if needed */
    if (nn >= cache_cap) {
        size_t new_cap = nn + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute up to nn, filling cache entries */
    for (size_t i = cache_limit + 1; i <= nn; ++i) {
        seq_t val;
        switch (i & 3) {
        case 0:
            val = 0;
            break;
        case 1:
            val = 2 * cache[(i - 1) >> 2];
            break;
        case 2: {
            size_t m = (i - 2) >> 2;
            seq_t sum = 0;
            for (size_t k = 0; k <= m; ++k) {
                sum += cache[k] * cache[m - k];
            }
            val = sum;
        } break;
        case 3:
            val = 0;
            break;
        }
        cache[i] = val;
    }
    cache_limit = nn;
    return cache[nn];
}
