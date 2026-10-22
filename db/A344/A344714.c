/*
 * A344714  Numbers k such that A008477(k) > k.
 * https://oeis.org/A344714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1504, u32 n <= 9297, u64 n <= 9297,
 *            u128 n <= 9297, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A344714: Numbers k such that
   A008477(k) = ∏ (exponent of p in k)^p  is greater than k.
   a(1) = 8, a(2) = 25, ...
   The routine searches integers in increasing order, tests the
   condition using the factor_u64 helper, and memoises the found terms
   so that any n can be answered efficiently. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */
#include "../utils/factor_u64.h"

/* Memoisation storage for previously found terms */
static seq_t  *seq_terms   = NULL;   /* dynamically grown array of values */
static size_t  seq_capacity = 0;     /* bytes allocated for seq_terms */
static size_t  seq_count    = 0;     /* number of stored terms */
static idx_t   next_search  = 1;     /* next integer to test */

/* Ensure the memoisation array can hold at least 'need' terms. */
static void
ensure_capacity(size_t need)
{
    if (need <= seq_capacity) return;
    size_t newcap = seq_capacity ? seq_capacity << 1 : 16;
    while (newcap < need) newcap <<= 1;
    /* realloc returns void* in C; explicit cast for C++ */
    seq_terms = (seq_t *)realloc(seq_terms, newcap * sizeof(seq_t));
    seq_capacity = newcap;
}

/* Return true if A008477(k) > k.  The product is computed in a 128‑bit
   integer with an early exit as soon as the product exceeds k, so the
   routine works even when the full product would overflow 64 bits. */
static bool
good_candidate(idx_t k)
{
    /* factorization */
    uint64_t primes[64], exps[64];
    size_t   cnt;
    factor_u64((uint64_t)k, primes, exps, &cnt);

    __int128 prod = 1;
    __int128 N    = k;          /* compare to k */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (e == 1) continue;   /* e^p = 1, contributes nothing */

        /* compute e^p safely, aborting early if it already exceeds N */
        __int128 val = 1;
        for (uint64_t j = 0; j < p; ++j) {
            if (val > N / e) return true;  /* product would already exceed k */
            val *= e;
            if (val > N) return true;      /* val alone exceeds k */
        }

        if (prod > N / val) return true;    /* prod * val would exceed k */
        prod *= val;
        if (prod > N) return true;          /* final check */
    }
    return prod > N;
}

/* Return the n-th term (n ≥ 1).  Zero is returned for n == 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not defined, but harmless */

    /* If already cached, just return it. */
    if ((size_t)n <= seq_count) return seq_terms[(size_t)n - 1];

    /* Grow the cache to hold at least n terms. */
    ensure_capacity((size_t)n);

    /* Search forward until we have at least n terms. */
    while (seq_count < n) {
        while (!good_candidate(next_search)) ++next_search;
        seq_terms[seq_count++] = (seq_t)next_search; /* cast fits in seq_t */
        ++next_search;
    }
    return seq_terms[(size_t)n - 1];
}
