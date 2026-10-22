/*
 * A317534  Numbers k such that the poset of factorizations of k, ordered by
 *          refinement, is not a lattice.
 * https://oeis.org/A317534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_non_lattice_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS sequence A317534: numbers k such that the poset of
 * factorizations of k, ordered by refinement, is not a lattice.
 * A number k is in the sequence iff
 *   (i)   some prime divisor has exponent ≥ 5              (prime power p^e, e≥5)
 *   (ii)  there exists a prime divisor with exponent ≥ 3
 *         and at least one other distinct prime divisor
 *   (iii) there exists a prime divisor with exponent ≥ 2
 *         and at least two other distinct prime divisors
 * The sequence is enumerated with offset 1 (a(1)=24). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_non_lattice_uint64.h"

/* cache of already computed terms */
static seq_t *cache = NULL;     /* array of cached sequence values */
static size_t cache_len = 0;    /* number of valid terms in cache */
static size_t cache_cap = 0;    /* allocated capacity of the array */

/* the next integer to test for the non‑lattice property */
static uint64_t next_candidate = 2;

/* append a value to the cache, expanding it if necessary */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* behaviour for n < 1 is irrelevant */

    /* initialise cache if first use */
    if (cache_len == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        if (is_non_lattice_uint64(next_candidate)) {
            cache_append((seq_t)next_candidate);
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
