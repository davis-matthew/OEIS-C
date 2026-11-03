/*
 * A282323  Lesser of twin primes congruent to 17 (mod 30).
 * https://oeis.org/A282323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that p and p+2 are prime and p % 30
 *            == 17
 * Range:     u8 n <= 5, u16 n <= 283, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 61 terms through n = 2874 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Static cache for the sequence values */
static seq_t *cache = NULL;         /* dynamic array of found terms   */
static size_t cache_len = 0;        /* number of terms stored         */
static size_t cache_cap = 0;        /* current capacity of the array  */

/* Next candidate with residue 17 mod 30 to test */
static seq_t next_candidate = 17;   /* starts at the first possible term */

/* Return the nth element of sequence A282323 (offset 1). */
seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0 for safety. */
    if (n == 0) return 0;

    /* Generate terms until we have at least n entries. */
    while (cache_len < n) {
        /* Test whether the current candidate and its twin are prime. */
        if (isPrimeGeneric(next_candidate) &&
            isPrimeGeneric(next_candidate + 2)) {
            /* Append candidate to the cache. */
            append_candidate(&cache, &cache_len, &cache_cap,
                             next_candidate);
        }
        /* Move to the next residue 17 mod 30. */
        next_candidate += 30;
    }

    /* Return the required element (array is zero‑indexed). */
    return cache[n - 1];
}
