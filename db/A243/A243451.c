/*
 * A243451  Primes of the form n^2 + 16.
 * https://oeis.org/A243451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 837 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: primes of the form n^2 + 16 (OEIS A243451) */

#include <stdlib.h>
#include <stdbool.h>
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

/* Cached generation of the sequence */
static seq_t *cache = NULL;     /* array holding discovered primes */
static size_t cache_len = 0;    /* number of primes stored */
static size_t cache_cap = 0;    /* allocated capacity of the array */
static idx_t next_i = 1;        /* next n to test for n^2+16 */

/* Grow the cache to at least capacity 'need' */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Initialise cache lazily */
    if (!cache) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
        cache_len = 0;
        next_i = 1;
    }

    /* Extend the cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        seq_t val = (seq_t)next_i * (seq_t)next_i + (seq_t)16;
        bool prime;
        if (val <= (seq_t)UINT64_MAX) {
            /* Fast 64‑bit primality test for small values */
            uint64_t v64 = (uint64_t)val;
            /* Use the provided generic test which works for 64‑bit as well */
            prime = isPrimeGeneric(v64);
        } else {
            /* For larger values use the generic primality test for seq_t */
            prime = isPrimeGeneric(val);
        }
        if (prime) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_i;
    }

    return cache[n - 1];
}
