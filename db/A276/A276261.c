/*
 * A276261  Centered 21-gonal primes.
 * https://oeis.org/A276261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 38 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): primes of the form (21*k^2 + 21*k + 2)/2.  a(n) is the
   n-th such prime, for n ≥ 1.  The function builds the sequence as
   needed and caches its values for subsequent calls. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Cache of found primes.  Static state persists across calls. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    /* Last k that was examined.  Initialized to (idx_t)-1 so that
       the first increment yields k = 0. */
    static idx_t last_k = (idx_t)-1;

    if (n == 0)     /* offset not used – behaviour irrelevant for n=0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while ((idx_t)cache_size < n)
    {
        idx_t k = ++last_k;
        seq_t kt = (seq_t)k;
        /* Compute (21*k^2 + 21*k + 2)/2 in seq_t. */
        seq_t numerator = kt * kt * 21 + kt * 21 + 2;
        seq_t value      = numerator / 2;
        if (isPrimeGeneric(value))
            append_candidate(&cache, &cache_size, &cache_cap, value);
    }
    return cache[n - 1];
}
