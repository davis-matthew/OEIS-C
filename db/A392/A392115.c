/*
 * A392115  Numbers k such that 6*k - (greatest prime < 6*n) > (least prime >
 *          6*n) - 6*k.
 * https://oeis.org/A392115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that * 12*k > (greatest prime < 6*k) +
 *            (least prime > 6*k)
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* I.e. 6*k - gp > lp - 6*k where gp and lp are the nearest primes
 * to 6*k from below and above, respectively.                                   */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Memoised cache of found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* allocated capacity of cache array   */
static idx_t   last_k   = 0;   /* largest k already examined */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        while (new_cap < need) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute the next k that satisfies the inequality.  The function
   updates last_k and appends the new term to cache. */
static void find_next_term(void)
{
    idx_t k = last_k + 1;              /* start searching just after last_k */

    for (;;) {
        /* 6*k */
        seq_t sixk = (seq_t)6 * (seq_t)k;

        /* greatest prime < 6*k */
        seq_t g = sixk - 1;
        while (!is_prime_big(g)) --g;

        /* least prime > 6*k */
        seq_t l = sixk + 1;
        while (!is_prime_big(l)) ++l;

        /* inequality: 12*k > g + l */
        seq_t left  = (seq_t)12 * (seq_t)k;
        seq_t right = g + l;
        if (left > right) {
            /* Found a valid k */
            if (cache_len == cache_cap) {
                ensure_cache_capacity(cache_len + 1);
            }
            cache[cache_len++] = (seq_t)k;
            last_k = k;
            break;
        }
        ++k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) is defined */

    /* Compute terms until we have the nth one. */
    while (cache_len < n) {
        find_next_term();
    }
    return cache[n - 1];
}
