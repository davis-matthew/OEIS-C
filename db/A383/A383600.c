/*
 * A383600  Expansion of 1/( (1-x)^3 * (1-9*x) )^(1/4).
 * https://oeis.org/A383600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((10*n-7)*a(n-1) - 9*(n-1)*a(n-2)) / n with a(0)=1,
 *            a(1)=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <type_traits>
#endif

static seq_t *cache = NULL;        /* pointer to cached terms   */
static size_t cache_len = 0;       /* number of terms in cache */

/* Return the n-th term of the sequence A383600. */
seq_t a(idx_t n)
{
    /* Ensure cache contains a(0) and a(1). */
    if (cache_len < 2) {
        cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        cache_len = 2;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* Extend cache if necessary. */
    if (cache_len <= (size_t)n) {
        /* Grow the array to hold terms up to n. */
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));

        /* Compute new terms using the recurrence. */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            idx_t ii = (idx_t)i;                     /* loop index as idx_t */
            unsigned long long coef1 = 10ull * ii - 7ull;  /* 10*i - 7      */
            unsigned long long coef2 = 9ull * (ii - 1);    /* 9*(i-1)       */

#ifdef __cplusplus
            if constexpr (std::is_integral<seq_t>::value) {
                /* Integral seq_t variant – use 128‑bit arithmetic for safety. */
                __uint128_t part1 = (__uint128_t)cache[i - 1] * coef1;
                __uint128_t part2 = (__uint128_t)cache[i - 2] * coef2;
                __uint128_t numer = part1 - part2;
                cache[i] = (seq_t)(numer / (unsigned long long)ii);
            } else {
                /* Arbitrary‑precision seq_t – rely on its operators. */
                seq_t part1 = cache[i - 1] * (seq_t)coef1;
                seq_t part2 = cache[i - 2] * (seq_t)coef2;
                cache[i] = (part1 - part2) / ii;
            }
#else
            /* C implementation – __uint128_t is sufficient for all tested values. */
            __uint128_t part1 = (__uint128_t)cache[i - 1] * coef1;
            __uint128_t part2 = (__uint128_t)cache[i - 2] * coef2;
            __uint128_t numer = part1 - part2;
            cache[i] = (seq_t)(numer / (unsigned long long)ii);
#endif
        }
        cache_len = n + 1;
    }
    return cache[(size_t)n];
}
