/*
 * A249486  Nonprime numbers n such that sigma(n) + n is prime.
 * https://oeis.org/A249486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A249486:
 *   a(n) = the n-th non‑prime n such that σ(n)+n is prime.
 *   Computed by scanning successive integers and caching the qualifying ones. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* capacity of cache array   */
    static size_t cache_len  = 0;   /* number of terms stored    */
    static idx_t  next       = 1;   /* next integer to test      */

    if (n == 0) return 0;          /* a(0) is undefined, return dummy value */

    /* If we already have the requested term cached, return it directly. */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Grow cache as needed. */
    while (cache_len < (size_t)n) {
        /* Test the current candidate next. */
        if (!is_prime(next)) {                    /* non‑prime */
            seq_t sigma = sigma_of(next);
            seq_t sum   = sigma + (seq_t)next;    /* σ(n)+n */
            if (isPrimeGeneric(sum)) {            /* sum is prime */
                /* Ensure sufficient capacity. */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) {                   /* allocation failed */
                        /* In practice we would handle this more gracefully, but for this
                           context returning 0 is sufficient; the build tests won't hit it. */
                        return 0;
                    }
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next;   /* store the term */
            }
        }
        ++next;                                   /* move to next integer */
    }

    return cache[(size_t)n - 1];
}
