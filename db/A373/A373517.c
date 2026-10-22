/*
 * A373517  Expansion of e.g.f. exp(x/(1 - x^3)^(1/3)).
 * https://oeis.org/A373517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n^3 -36*n^2 +112*n -119)*a(n-3) * -
 *            2*(n-6)*(n-5)*(n-4)*(n-3)*(3*n^2-27*n+64)*a(n-6) * +
 *            4*(n-9)*(n-8)*(n-7)*(n-6)^3*(n-5)*(n-4)*(n-3)*a(n-9) * -
 *            (n-12)*(n-11)*(n-10)*(n-9)^2*(n-8)*(n-7)*(n-6)^2*(n-5)*(n-4)*(n-3)*a(n-12)
 *            * Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, * a(4)=9,
 *            a(5)=4...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const uint64_t init_vals[12] = {
    1ULL, 1ULL, 1ULL, 1ULL,        /* a(0)..a(3) */
    9ULL, 41ULL, 121ULL,          /* a(4)..a(6) */
    1401ULL, 11761ULL,            /* a(7)..a(8) */
    61489ULL, 864081ULL,          /* a(9)..a(10) */
    10597841ULL                  /* a(11) */
};

static seq_t *cache = NULL;       /* memoization table */
static size_t cache_cap = 0;      /* current capacity of cache */

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Quick return for very small n to avoid unnecessary allocation */
    if (n < 12)
        return (seq_t)init_vals[(size_t)n];

    /* Ensure cache has enough space */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(EXIT_FAILURE);          /* allocation failed */

        cache = new_cache;

        /* Fill the cache from current size up to n */
        for (size_t i = cache_cap; i <= n; ++i) {
            if (i < 12) {
                cache[i] = (seq_t)init_vals[i];
                continue;
            }

            idx_t ii = (idx_t)i;

            /* term1 = (4*n^3 - 36*n^2 + 112*n - 119) * a(n-3) */
            seq_t term1 = ((seq_t)4 * ii * ii * ii -
                           (seq_t)36 * ii * ii +
                           (seq_t)112 * ii -
                           (seq_t)119) *
                          cache[i - 3];

            /* term2 = 2*(n-6)*(n-5)*(n-4)*(n-3)*(3*n^2-27*n+64)*a(n-6) */
            seq_t term2 = 2 *
                          (seq_t)(ii - 6) *
                          (seq_t)(ii - 5) *
                          (seq_t)(ii - 4) *
                          (seq_t)(ii - 3) *
                          ((seq_t)3 * ii * ii -
                           (seq_t)27 * ii +
                           (seq_t)64) *
                          cache[i - 6];

            /* term3 = 4*(n-9)*(n-8)*(n-7)*(n-6)^3*(n-5)*(n-4)*(n-3)*a(n-9) */
            seq_t term3 = 4 *
                          (seq_t)(ii - 9) *
                          (seq_t)(ii - 8) *
                          (seq_t)(ii - 7) *
                          (seq_t)(ii - 6) *
                          (seq_t)(ii - 6) *
                          (seq_t)(ii - 6) *
                          (seq_t)(ii - 5) *
                          (seq_t)(ii - 4) *
                          (seq_t)(ii - 3) *
                          cache[i - 9];

            /* term4 = (n-12)*(n-11)*(n-10)*(n-9)^2*(n-8)*(n-7)*(n-6)^2*(n-5)*(n-4)*(n-3)*a(n-12) */
            seq_t term4 =
                (seq_t)(ii - 12) *
                (seq_t)(ii - 11) *
                (seq_t)(ii - 10) *
                ((seq_t)(ii - 9) * (seq_t)(ii - 9)) *
                (seq_t)(ii - 8) *
                (seq_t)(ii - 7) *
                ((seq_t)(ii - 6) * (seq_t)(ii - 6)) *
                (seq_t)(ii - 5) *
                (seq_t)(ii - 4) *
                (seq_t)(ii - 3) *
                cache[i - 12];

            cache[i] = term1 - term2 + term3 - term4;
        }

        cache_cap = new_cap;
    }

    return cache[(size_t)n];
}
