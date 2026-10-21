/*
 * A251663  E.g.f.: exp( 3*x*G(x)^2 ) / G(x), where G(x) = 1 + x*G(x)^3 is the
 *          g.f. A001764.
 * https://oeis.org/A251663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=11, and for n≥3
 *            2*(2*n‑1)*(9*n^2‑30*n+19)*a(n) =
 *            3*(81*n^4‑432*n^3+756*n^2‑393*n‑88)*a(n‑1) -
 *            27*(9*n^2‑12*n‑2)*a(n‑2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n) {
    /* Allocate space for the base cases if necessary */
    if (cache == NULL) {
        cache_cap = (n >= 2 ? n + 1 : 3);
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)11;
        cache_len = 3;
    }

    /* Grow the cache and compute missing terms */
    if (cache_len <= n) {
        if (cache_cap <= (size_t)n) {
            size_t newcap = cache_cap > 0 ? cache_cap : 1;
            while (newcap <= (size_t)n) newcap <<= 1;
            void *tmp = realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = (seq_t*)tmp;
            cache_cap = newcap;
        }

        for (idx_t i = cache_len; i <= n; ++i) {
#ifdef __SIZEOF_INT128__
            __uint128_t nn = i;
            __uint128_t n2 = nn * nn;
            __uint128_t n3 = n2 * nn;
            __uint128_t n4 = n3 * nn;
            __uint128_t p1 = 81 * n4 - 432 * n3 + 756 * n2 - 393 * nn - 88;
            __uint128_t p2 = 9 * n2 - 12 * nn - 2;
            __uint128_t denom = 2 * (2 * nn - 1) * (9 * n2 - 30 * nn + 19);
            __uint128_t term1 = 3 * p1 * (__uint128_t)cache[i-1];
            __uint128_t term2 = 27 * p2 * (__uint128_t)cache[i-2];
            cache[i] = (seq_t)((term1 - term2) / denom);
#else
            seq_t nn = (seq_t)i;
            seq_t n2 = nn * nn;
            seq_t n3 = n2 * nn;
            seq_t n4 = n3 * nn;
            seq_t p1 = 81 * n4 - 432 * n3 + 756 * n2 - 393 * nn - 88;
            seq_t p2 = 9 * n2 - 12 * nn - 2;
            seq_t denom = 2 * (2 * nn - 1) * (9 * n2 - 30 * nn + 19);
            seq_t num   = 3 * p1 * cache[i-1] - 27 * p2 * cache[i-2];
            cache[i]    = num / denom;
#endif
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
