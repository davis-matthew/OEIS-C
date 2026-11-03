/*
 * A385824  Primes p such that p + 10, p + 18, p + 24, p + 28 and p + 30 are
 *          also primes.
 * https://oeis.org/A385824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A385824:
   primes p such that p + 10, p + 18, p + 24, p + 28 and p + 30 are also primes. */
/* The function builds the sequence lazily, caching previously found terms
   so that calls with arbitrary n (not necessarily monotonic) are answered quickly. */

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

seq_t a(idx_t n)
{
    /* cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    /* last integer that was examined as a potential base prime */
    static idx_t last_checked = 1;

    if (n == 0) /* undefined for offset 1; return 0 */
        return (seq_t)0;

    /* extend cache until we have n terms */
    while (cache_len < (size_t)n) {
        /* look for next odd candidate */
        idx_t cand = ++last_checked;
        if ((cand & 1) == 0)                  /* skip evens */
            continue;
        if (cand % 3 == 2)                    /* p must be 1 (mod 3) */
            continue;
        if (cand % 5 == 0)                    /* avoid multiples of 5 */
            continue;
        if (!isPrimeGeneric((seq_t)cand))
            continue;

        /* check the five offsets */
        seq_t p  = (seq_t)cand;
        seq_t p10 = p + 10;
        seq_t p18 = p + 18;
        seq_t p24 = p + 24;
        seq_t p28 = p + 28;
        seq_t p30 = p + 30;

        if (!isPrimeGeneric(p10)) continue;
        if (!isPrimeGeneric(p18)) continue;
        if (!isPrimeGeneric(p24)) continue;
        if (!isPrimeGeneric(p28)) continue;
        if (!isPrimeGeneric(p30)) continue;

        /* all primes – store the base prime */
        if (cache_len + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = p;
    }

    return cache[(size_t)n - 1];
}
