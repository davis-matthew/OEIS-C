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
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* local helper: test whether a candidate satisfies the non‑lattice
 * condition.  The algorithm factors k by trial division, records the
 * prime exponents, and checks the three conditions listed above.   */
static bool is_non_lattice_uint64(uint64_t k)
{
    if (k <= 1) return false;

    uint64_t n = k;
    int exps[64];
    size_t cnt = 0;

    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        int e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        exps[cnt++] = e;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }

    /* remaining prime factor (if any) */
    if (n > 1) {
        exps[cnt++] = 1;
    }

    /* Condition (i) : exponent ≥ 5 */
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] >= 5)
            return true;

    /* Condition (ii) : exponent ≥ 3 and at least one other prime */
    if (cnt >= 2) {
        for (size_t i = 0; i < cnt; ++i)
            if (exps[i] >= 3)
                return true;
    }

    /* Condition (iii) : exponent ≥ 2 and at least two other primes */
    if (cnt >= 3) {
        for (size_t i = 0; i < cnt; ++i)
            if (exps[i] >= 2)
                return true;
    }

    return false;
}

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
        cache = mem_resize(seq_t, cache, cache_cap);
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
        cache = mem_new(seq_t, cache_cap);
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
