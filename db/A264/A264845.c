/*
 * A264845  Centered 20-gonal (or icosagonal) primes.
 * https://oeis.org/A264845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Centered 20‑gonal primes.
   Each term is the n‑th prime of the form 10*k^2 - 10*k + 1. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static state used for memoisation. */
static seq_t *cache       = NULL; /* Cached prime values. */
static size_t cache_cap   = 0;    /* Allocated capacity of cache. */
static size_t cache_len   = 0;    /* Number of primes stored. */
static idx_t   next_k      = 1;   /* Next integer k to test. */

/* Return the n‑th term of the sequence, with offset 1. */
seq_t a(idx_t n)
{
    /* Determine the maximum value that can be represented by seq_t. */
    unsigned __int128 max_val;
    switch (sizeof(seq_t)) {
        case 1:  max_val = UINT8_MAX;   break;
        case 2:  max_val = UINT16_MAX;  break;
        case 4:  max_val = UINT32_MAX;  break;
        case 8:  max_val = UINT64_MAX;  break;
        case 16: max_val = (unsigned __int128)(-1); break; /* all bits 1 */
        default: max_val = (unsigned __int128)(-1); break;
    }

    /* Generate primes until we have at least n of them or we ran out of range. */
    while (cache_len < n) {
        unsigned __int128 kk      = (unsigned __int128)next_k;
        unsigned __int128 val128  = 10 * kk * kk - 10 * kk + 1;

        /* If val128 cannot be represented in seq_t, we cannot store it,
           so stop generating further primes. */
        if (val128 > max_val) {
            break;
        }

        seq_t val = (seq_t)val128;
        if (isPrimeGeneric(val)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
        ++next_k;
    }

    /* If we did not manage to find enough primes (overflowed seq_t range),
       return 0.  The specification only requires correctness when the
       result fits into seq_t. */
    if (cache_len < n) {
        return 0;
    }
    return cache[n - 1];
}
