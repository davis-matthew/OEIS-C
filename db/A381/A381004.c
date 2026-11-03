/*
 * A381004  Primes ending in 777.
 * https://oeis.org/A381004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1000*A102343(n)+777
 * Range:     u8 none, u16 n <= 12, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 51 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102343(n) is the n-th prime congruent to 3 (mod 1000). */
/* This function iterates over candidates 1000*k+777 and tests primality. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

static seq_t *cache = NULL;          /* cached primes of the form 1000*k+777 */
static size_t cache_len = 0;          /* number of cached primes */
static size_t cache_cap = 0;          /* allocated capacity of cache   */
static idx_t  next_k = 1;             /* k of the next candidate to test */

/* Make sure the cache can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();               /* Out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th prime ending in 777. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* Offset is 1; for n=0 return 0 */

    /* Ensure the cache holds the first n primes. */
    while (cache_len < (size_t)n) {
        seq_t cand = (seq_t)next_k * (seq_t)1000 + (seq_t)777;
        if (isPrimeGeneric(cand)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = cand;
        }
        ++next_k;
    }
    return cache[n - 1];
}
