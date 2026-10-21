/*
 * A283759  Numbers whose Euler totient function is equal to the product of
 *          the number of divisors of their k first powers, for some k.
 * https://oeis.org/A283759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A283759 – Numbers whose Euler totient function equals
 *  the product of divisor counts of successive powers.
 *
 *  This implementation searches integers in increasing order,
 *  keeping the results in a static cache so that calls with any
 *  index ordering return the correct term.  Factorisation is done
 *  with the provided factor_u64 routine.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* factor_u64(n, primes, exps, &cnt) */

static seq_t *cache      = NULL;    /* array of found terms          */
static size_t cache_len  = 0;        /* number of terms stored       */
static size_t cache_cap  = 0;        /* current capacity             */
static idx_t  next_cand  = 2;        /* next integer to test         */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;

    if (cache == NULL)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));

    cache_cap = new_cap;
}

/* Return true iff φ(n) = ∏_{i=1..k} d(n^i) for some k ≥ 1. */
static bool satisfies(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* φ(n) = n * Π (1 - 1/p)  =>  φ = φ/p*(p-1) for each prime factor. */
    seq_t phi = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i)
        phi = phi / (seq_t)primes[i] * ((seq_t)primes[i] - 1);

    /* Check increasing k until product exceeds φ. */
    seq_t prod = 1;
    for (idx_t k = 1; ; ++k) {
        seq_t factor = 1;
        for (size_t i = 0; i < cnt; ++i)
            factor *= ((seq_t)exps[i] * k + 1);

        prod *= factor;
        if (prod == phi) return true;
        if (prod > phi) break;
        if (prod == 0)   /* overflow guard – should not happen for needed range */
            break;
    }
    return false;
}

/* Return the n‑th term, 1‑based index (a(1) = 3). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for the sequence */

    /* Extend cache until it contains n terms. */
    while (cache_len < n) {
        if (satisfies(next_cand)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_cand;
        }
        ++next_cand;
    }
    return cache[n - 1];
}
