/*
 * A345744  Numbers k such that k and k+1 are products of at least 5 primes.
 * https://oeis.org/A345744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   bigomega_u64_to_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345744: numbers k such that k and k+1 are products of at least
 * five prime factors (counted with multiplicity).  The function
 * a(idx_t n) returns the n-th term (a(1) = 728).  It is computed by an
 * incremental search and memoises the results in a dynamic array.  The
 * implementation uses only integer arithmetic and a simple prime‑factor
 * counter suitable for 64‑bit values; this is sufficient for the
 * ranges needed by the tests.  For arbitrary‑precision builds the
 * returned seq_t value is constructed from a uint64_t candidate. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega_u64_to_i32.h"
#include "../utils/mem.h"

/*------------------------------------------------------------*/
/*    Memoisation storage                                   */
/*------------------------------------------------------------*/
static seq_t *cache = NULL;     /* array of cached terms            */
static size_t cache_len = 0;    /* number of terms stored           */
static size_t cache_cap = 0;    /* allocated capacity               */
static uint64_t last_checked = 0; /* last candidate examined */

/* Ensure we have enough room for an additional term. */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        /* explicit cast for C++ compliance */
        cache = mem_resize(seq_t, cache, cache_cap);
    }
}

/* Generate terms until cache contains at least n elements. */
static void generate_until(idx_t n)
{
    uint64_t cand = last_checked + 1;
    while (cache_len < n) {
        if (bigomega_u64_to_i32(cand) >= 5 && bigomega_u64_to_i32(cand + 1) >= 5) {
            ensure_cache_capacity();
            cache[cache_len++] = (seq_t)cand;
        }
        ++cand;
    }
    last_checked = cand - 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; arbitrary */
    generate_until(n);
    return cache[n - 1];
}
