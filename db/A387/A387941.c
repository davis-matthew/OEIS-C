/*
 * A387941  Positive numbers k such that gpf(k)^2 < 2*k where gpf = A006530.
 * https://oeis.org/A387941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all positive integers k whose largest prime
   factor g satisfies g^2 < 2*k.  No closed formula is known, so the
   implementation enumerates integers progressively, keeps a cache of
   the terms found, and expands a sieve for the largest prime factor
   as needed. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* ----------  Cache for the sequence values  ---------- */
    static seq_t *cache      = NULL;   /* array of found terms          */
    static size_t cache_len  = 0;      /* number of terms stored        */
    static size_t cache_cap  = 0;      /* capacity of the cache array   */

    /* ----------  Sieve for the largest prime factor  ---------- */
    static uint64_t *lpf      = NULL;   /* lpf[i] = largest prime factor of i (0 if i<2) */
    static size_t lpf_cap      = 0;      /* array size (indexable up to lpf_cap-1)       */

    static uint64_t max_checked = 0;    /* highest integer already processed */

    /* Ensure cache has room for at least one more term. */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = (cache_cap ? cache_cap * 2 : 64);
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (p == NULL) abort();
        cache = p;
        cache_cap = new_cap;
    }

    /* -------  Return immediately if we already have the term  ------- */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* -------  Produce more terms until we have at least n  ------- */
    while (cache_len < n) {
        /* Determine new limit for the sieve */
        uint64_t new_limit = (lpf_cap > 0 ? lpf_cap * 2 : 64);
        if (new_limit < 2 * n) new_limit = 2 * n;

        /* Reallocate sieve array to new_limit+1 entries */
        uint64_t *nl = (uint64_t *)realloc(lpf, (new_limit + 1) * sizeof(uint64_t));
        if (nl == NULL) abort();
        lpf = nl;
        lpf_cap = new_limit + 1;

        /* Initialise to zero */
        for (uint64_t i = 0; i <= new_limit; ++i) {
            lpf[i] = 0;
        }

        /* Sieve: for each prime, set it as the largest prime factor for all its multiples */
        for (uint64_t i = 2; i <= new_limit; ++i) {
            if (lpf[i] == 0) {               /* i is prime */
                for (uint64_t j = i; j <= new_limit; j += i) {
                    lpf[j] = i;               /* overwrite to keep the largest prime seen so far */
                }
            }
        }

        /* Process numbers from max_checked+1 up to new_limit */
        for (uint64_t k = max_checked + 1; k <= new_limit; ++k) {
            uint64_t g = (k == 1) ? 1 : lpf[k];   /* gpf(1) is treated as 1 */
            unsigned __int128 lhs = (unsigned __int128)g * g;
            unsigned __int128 rhs = (unsigned __int128)2 * k;
            if (lhs < rhs) {
                /* Ensure cache is large enough (might need another expansion) */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = (cache_cap ? cache_cap * 2 : 64);
                    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (p == NULL) abort();
                    cache = p;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)k;
            }
        }

        max_checked = new_limit;
    }

    return cache[n - 1];
}
