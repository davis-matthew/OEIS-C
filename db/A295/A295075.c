/*
 * A295075  Numbers k such that d + k/d is never prime for any divisor d of k.
 * https://oeis.org/A295075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9272 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the n-th integer k such that for every divisor d of k,
   the sum d + k/d is never prime.  The function builds a cache of
   found values and returns the (n-1)-indexed element.  It uses simple
   divisor enumeration; thus it is O(N sqrt N) but fast enough for
   n ≤ 10 000. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

static idx_t *cache = NULL;          /* cache of found terms */
static idx_t cache_len = 0;          /* number of elements stored */
static idx_t cache_cap = 0;          /* allocated capacity */
static uint64_t last_checked = 0;    /* last number examined */

/* Grow the cache to at least 'needed' slots.  Safe for both C and C++ */
static void ensure_cache_size(idx_t needed)
{
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    idx_t *tmp = (idx_t *)realloc(cache, sizeof(idx_t) * new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return true iff all divisor sums d + k/d are composite (or 1). */
static bool satisfies(uint64_t k)
{
    for (uint64_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            seq_t s = (seq_t)d + (seq_t)(k / d);
            if (isPrimeGeneric(s)) return false;
        }
    }
    return true;
}

/* Public sequence function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0, return 0 safely */
    /* Ensure cache holds at least n elements */
    while (cache_len < n) {
        ensure_cache_size(cache_len + 64);
        ++last_checked;
        if (satisfies(last_checked))
            cache[cache_len++] = last_checked;
    }
    return (seq_t)cache[n - 1];
}
