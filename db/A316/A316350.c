/*
 * A316350  Positive integers x that are x/log(x) smooth, that is, if a prime
 *          p divides x, then p <= x/log(x).
 * https://oeis.org/A316350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_x_log_smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algo: brute‑force construction of A316350.
   For each integer x we test whether every prime factor p of x
   satisfies p <= floor(x / log(x)).  The smallest prime factor
   array (spf) is constructed incrementally; a cache of the
   discovered sequence terms is maintained for O(1) retrieval. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_x_log_smooth.h"

/* Fill an already allocated array with smallest prime factors for
   all integers up to `limit`. */
static void
fill_spf(uint64_t limit, uint64_t *spf)
{
    for (uint64_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (uint64_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if (i > limit / i) continue;  /* avoid overflow i*i */
            for (uint64_t j = i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* --- cache of discovered terms ----------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* --- SPF table to factor numbers quickly ------------------------ */
    static uint64_t *spf = NULL;
    static uint64_t spf_limit = 0; /* highest index filled in spf array */

    /* --- next integer to test -------------------------------------- */
    static uint64_t current_x = 1;

    /* Produce terms until we have at least `n` of them. */
    while (cache_len < (size_t)n) {
        /* Grow the SPF table if necessary. */
        if (current_x > spf_limit) {
            uint64_t new_limit = current_x * 2 + 100;            /* some slack */
            uint64_t *new_spf = (uint64_t *)malloc((new_limit + 1) * sizeof(uint64_t));
            if (!new_spf) abort();
            fill_spf(new_limit, new_spf);
            free((void *)spf);
            spf = new_spf;
            spf_limit = new_limit;
        }

        /* Check if current_x is x/log(x)‑smooth. */
        if (is_x_log_smooth(current_x, spf)) {
            /* Ensure sufficient space in the cache. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
                free((void *)cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)current_x;
        }
        ++current_x;
    }

    return cache[(size_t)n - 1];
}
