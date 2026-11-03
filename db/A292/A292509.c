/*
 * A292509  Primes of the form k^2 + 23*k + 23.
 * https://oeis.org/A292509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 155, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 64 terms through n = 9667 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime of the form k^2 + 23*k + 23 (OEIS A292509),
   with offset 1: a(1) = 23, a(2) = 47, … This implementation does
   a memoised incremental search using the helper isPrimeGeneric. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* Cache of already found values. */
    static seq_t *cache = NULL;   /* array of cached values          */
    static idx_t cached   = 0;    /* number of values stored         */
    static idx_t cap      = 0;    /* current capacity of the cache   */
    static idx_t last_k   = 0;    /* last k value examined           */

    if (n == 0)   /* out of bounds – caller should not request n<1 */
        return 0;

    /* Allocate or grow cache as needed. */
    if (cached < n) {
        if (cache == NULL) {
            cap = 32;
            cache = mem_new(seq_t, cap);
            if (cache == NULL) return 0;   /* allocation failure (rare) */
        } else if (cached >= cap) {
            cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, cap);
            if (tmp == NULL) return 0;     /* allocation failure (rare) */
            cache = tmp;
        }

        /* Continue searching until the required number of terms is found. */
        while (cached < n) {
            seq_t kk = (seq_t)last_k;
            seq_t val = kk * kk + (seq_t)23 * kk + (seq_t)23;
            if (isPrimeGeneric(val)) {
                if (cached >= cap) {        /* buffer full – expand */
                    cap <<= 1;
                    seq_t *tmp = mem_resize(seq_t, cache, cap);
                    if (tmp == NULL) return 0;
                    cache = tmp;
                }
                cache[cached++] = val;
            }
            ++last_k;
        }
    }
    return cache[n - 1];
}
