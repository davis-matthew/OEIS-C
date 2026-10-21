/*
 * A251675  Number of (n+2)X(1+2) 0..2 arrays with no row, column, diagonal or
 *          antidiagonal in any 3X3 subblock summing to 2 or 4.
 * https://oeis.org/A251675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 20, u64 n <= 44, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A251675: number of (n+2)×(1+2) 0..2 arrays with no row, column, 
 * diagonal or antidiagonal in any 3×3 subblock summing to 2 or 4.
 *
 * Recurrence (valid for n>15):
 *   a(n) = 2*a(n-1) - a(n-2) + 11*a(n-3) - 13*a(n-4) + 8*a(n-5)
 *          - 32*a(n-6) + 16*a(n-7) - 18*a(n-8) + 24*a(n-9)
 *          + 20*a(n-10) + 16*a(n-11) - 8*a(n-12) - 16*a(n-13) - 8*a(n-14)
 * Initial values:
 *   a(1)=293, a(2)=489, a(3)=953, a(4)=2257, a(5)=5151, a(6)=12063,
 *   a(7)=29839, a(8)=72345, a(9)=176143, a(10)=435429, a(11)=1069885,
 *   a(12)=2629463, a(13)=6489171, a(14)=15986259, a(15)=39377317.
 *
 * The function is memoised and works in O(n) time per call (amortised).
 * It uses only integer arithmetic and standard library functions, so it
 * is valid C11 and C++17 for any seq_t/idx_t definition.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: grow the cache up to at least new_cap elements. */
static void
ensure_capacity(size_t new_cap, size_t *cap, seq_t **ptr)
{
    if (*cap < new_cap) {
        size_t new_size = new_cap;
        seq_t *tmp = (seq_t *)realloc(*ptr, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failure – abort. */
            abort();
        }
        *ptr = tmp;
        *cap = new_size;
    }
}

seq_t a(idx_t n)
{
    /* Static memoisation tables. */
    static seq_t *cache      = NULL;   /* terms a(1)...a(cache_len)  */
    static size_t cache_len  = 0;      /* number of terms stored     */
    static size_t cache_cap  = 0;      /* allocated capacity         */

    /* Initial values for a(1) … a(15).  Stored in 0‑based indexing. */
    static const seq_t init[15] = {
        293ULL, 489ULL, 953ULL, 2257ULL, 5151ULL, 12063ULL,
        29839ULL, 72345ULL, 176143ULL, 435429ULL,
        1069885ULL, 2629463ULL, 6489171ULL, 15986259ULL,
        39377317ULL
    };

    /* Coefficients for the recurrence (a(n-1) … a(n-14)). */
    static const int coeff[14] = {
        2,  -1, 11, -13, 8, -32, 16, -18, 24, 20, 16, -8, -16, -8
    };

    /* On first call, build the initial cache. */
    if (cache_len == 0) {
        /* Allocate at least 15 entries. */
        ensure_capacity(15, &cache_cap, &cache);
        for (size_t i = 0; i < 15; ++i) {
            cache[i] = init[i];
        }
        cache_len = 15;
    }

    /* If the requested term is already known, return it immediately. */
    if ((idx_t)cache_len >= n) {
        return cache[(size_t)(n - 1)];
    }

    /* Grow the cache to at least the requested index. */
    ensure_capacity((size_t)n, &cache_cap, &cache);

    /* Compute terms a(cache_len+1) … a(n) using the recurrence. */
    for (idx_t idx = (idx_t)cache_len + 1; idx <= n; ++idx) {
        seq_t pos = 0;   /* sum of positive contributions   */
        seq_t neg = 0;   /* sum of negative contributions   */

        /* idx is 1‑based; we need a(idx-1), …, a(idx-14). */
        for (int j = 1; j <= 14; ++j) {
            seq_t term = cache[(size_t)(idx - 1 - j)]; /* a(idx-j)   */
            int c = coeff[j - 1];
            if (c > 0) {
                pos += term * (seq_t)c;
            } else {
                neg += term * (seq_t)(-c);
            }
        }
        cache[(size_t)(idx - 1)] = pos - neg;
    }

    cache_len = (size_t)n;
    return cache[(size_t)(n - 1)];
}
