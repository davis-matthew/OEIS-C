/*
 * A335655  Numbers k such that Omega(k+1) = Omega(k) + m, where Omega(k) =
 *          A001222(k) is the number of prime factors of k with multiplicity,
 *          case m = 3.
 * https://oeis.org/A335655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335655: Numbers k such that Omega(k+1) = Omega(k) + 3
   (Omega(n) = A001222(n) is the number of prime factors of n with multiplicity) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"

/* Cached sequence values, growing as needed. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next integer to test in the scan. */
static idx_t next_k = 1;

/* Grow the cache array when we need more space. */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1;                  /* double */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Bypass for n == 0 (though the offset starts at 1). */
    if (n == 0) return 0;

    /* Continue scanning until we have at least n terms cached. */
    while (cache_len < n) {
        /* Compute Ω(next_k) and Ω(next_k+1) using the helper. */
        seq_t om_k   = bigomega(next_k);
        seq_t om_k1  = bigomega(next_k + 1);

        if (om_k1 == om_k + (seq_t)3) {
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
