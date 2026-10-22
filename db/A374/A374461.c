/*
 * A374461  Numbers k such that k and k+1 are both nonsquarefree exponentially
 *          odd numbers (A374459).
 * https://oeis.org/A374461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 372, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A374461:
   Numbers k such that k and k+1 are both nonsquarefree exponentially odd
   numbers (A374459).  The function performs a brute‑force search, caching
   found terms so that repeated calls are O(1).  The search uses the
   supplied factorisation routine fact_u64 and realloc logic from the
   utilities. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>                     /* for NULL */

/* Utility prototypes supplied by the build system */
#include "../utils/factor_u64.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Memoisation cache for the sequence */
static seq_t  *cache      = NULL;   /* pointer to array of seq_t terms   */
static size_t  cache_cap  = 0;      /* current allocated capacity        */
static size_t  cache_sz   = 0;      /* number of terms stored            */
static uint64_t last_test = 1;      /* next integer to examine           */

/*---------------------------------------------------------------------------*
 *  Test whether a number is “nonsquarefree exponentially odd”:
 *    • all prime exponents are odd,
 *    • at least one exponent is >= 3.
 *---------------------------------------------------------------------------*/
static bool expodd_nonsqf(uint64_t v)
{
    if (v < 2) return false; /* 1 is squarefree */

    uint64_t pr[64], ex[64];
    size_t   cnt;

    factor_u64(v, pr, ex, &cnt);

    bool nonsqf = false;
    for (size_t i = 0; i < cnt; ++i) {
        if ((ex[i] & 1u) == 0)      /* even exponent → not exponentially odd */
            return false;
        if (ex[i] >= 3)             /* at least one exponent >= 3            */
            nonsqf = true;
    }
    return nonsqf;
}

/*---------------------------------------------------------------------------*
 *  true if both k and k+1 are nonsquarefree exponentially odd
 *---------------------------------------------------------------------------*/
static inline bool good_k(uint64_t k)
{
    return expodd_nonsqf(k) && expodd_nonsqf(k + 1);
}

/*---------------------------------------------------------------------------*
 *  Grow the cache array as needed.  Uses seq_alloc/seq_realloc to ensure
 *  constructors are called for arbitrary‑precision types.
 *---------------------------------------------------------------------------*/
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < need) new_cap *= 2;

    if (cache == NULL) {
        cache = (seq_t*)seq_alloc(new_cap);
    } else {
        seq_t *tmp = (seq_t*)seq_realloc(cache, cache_cap, new_cap);
        cache = tmp;
    }
    cache_cap = new_cap;
}

/*---------------------------------------------------------------------------*
 *  Ensure that at least `n` terms of the sequence are known.
 *---------------------------------------------------------------------------*/
static void ensure_terms(idx_t n)
{
    if ((size_t)n <= cache_sz) return;            /* already enough */

    while (cache_sz < (size_t)n) {
        if (good_k(last_test)) {
            /* store the term */
            grow_cache(cache_sz + 1);
            cache[cache_sz++] = (seq_t)last_test;
        }
        ++last_test;
    }
}

/*===========================================================================*
 *  The required exported function.                                         *
 *===========================================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence offset is 1 */
    ensure_terms(n);
    return cache[(size_t)(n - 1)];
}
