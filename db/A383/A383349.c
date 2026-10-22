/*
 * A383349  Numbers that have the same set of digits as the sum of 4th powers
 *          of its digits.
 * https://oeis.org/A383349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_mask_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calc A383349:
   a(n) is the nth integer whose set of decimal digits equals the set of
   decimal digits of the sum of the fourth powers of its digits.
   Off‑by‑one: a(1) = 0, a(2) = 1, ...                      */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_mask_ull.h"

static const unsigned long long pow4[10] = {
    0ULL,      /* 0^4 */
    1ULL,      /* 1^4 */
    16ULL,     /* 2^4 */
    81ULL,     /* 3^4 */
    256ULL,    /* 4^4 */
    625ULL,    /* 5^4 */
    1296ULL,   /* 6^4 */
    2401ULL,   /* 7^4 */
    4096ULL,   /* 8^4 */
    6561ULL    /* 9^4 */
};

/* Sum of fourth powers of the decimal digits of x */
static unsigned long long sum_fourth_powers_ull(unsigned long long x)
{
    unsigned long long sum = 0ULL;
    while (x != 0ULL)
    {
        unsigned d = (unsigned)(x % 10ULL);
        sum += pow4[d];
        x /= 10ULL;
    }
    return sum;
}

/* Cache for sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static unsigned long long next_candidate = 0ULL;

/* Grow the cache at least to new_cap entries */
static void grow_cache(size_t new_cap)
{
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);    /* Out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined; safe guard */

    /* Ensure cache has at least n terms */
    while (cache_len < (size_t)n)
    {
        unsigned long long cand = next_candidate;
        /* Find next qualifying number */
        while (1)
        {
            unsigned long long sum = sum_fourth_powers_ull(cand);
            unsigned mask_cand = digit_mask_ull(cand);
            unsigned mask_sum  = digit_mask_ull(sum);

            if (mask_cand == mask_sum)
            {
                /* Only store if value fits in seq_t */
                if (cand <= (unsigned long long)(seq_t)-1)
                {
                    if (cache_len == cache_cap)
                        grow_cache(cache_cap ? (cache_cap * 2) : 16);
                    cache[cache_len++] = (seq_t)cand;
                    if (cache_len >= (size_t)n)
                    {
                        next_candidate = cand + 1ULL;
                        break;
                    }
                }
            }
            cand++;
        }
    }
    return cache[n - 1];
}
