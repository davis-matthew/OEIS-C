/*
 * A397248  Square roots of terms in A397247.
 * https://oeis.org/A397248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer m such that m² splits into two
 *            perfect squares (allowing leading zeros in the right part)
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/pow10.h"

/* Cache of found terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* current capacity */
static size_t cache_len = 0;   /* number of terms stored */
static idx_t tested_m = 0;     /* last integer m examined */

/* Test whether an integer is a perfect square */
static bool is_square_seq(seq_t x)
{
    seq_t r = floor_sqrt(x);
    return (r * r) == x;
}

/* Grow the cache when it runs out of room */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Make sure that at least n terms are stored in the cache */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;

    /* Scan successive integers until we have enough terms */
    while (cache_len < (size_t)n) {
        tested_m++;                      /* next integer to test */
        seq_t m = (seq_t)tested_m;
        seq_t sq = m * m;

        /* Determine decimal length of sq */
        seq_t temp = sq;
        size_t digits = 1;
        while (temp >= 10) {
            temp /= 10;
            digits++;
        }

        /* Try all non‑trivial splits */
        for (size_t k = 1; k < digits; ++k) {
            seq_t powk = pow10((idx_t)k);
            seq_t right = sq % powk;      /* lower k digits (allowing leading zeros) */
            seq_t left  = sq / powk;      /* remaining higher digits */

            if (is_square_seq(left) && is_square_seq(right)) {
                if (cache_len >= cache_cap)
                    grow_cache();
                cache[cache_len++] = m;
                break;                    /* stop at first valid split */
            }
        }
    }
}

/* External function required by the project */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined, but avoid UB */

    ensure_terms(n);                 /* compute if necessary */
    return cache[(size_t)n - 1];
}
