/*
 * A253785  Composite numbers n = prime(i_1) * ... * prime(i_k), prime(i_1) <=
 *          prime(i_2) <= ... <= prime(i_k), with at least one pair of
 *          successive prime factors (when sorted into monotonic order) where
 *          the latter prime factor is greater than the square of the former:
 *          prime(i_x)^2 < prime(i_{x+1}), for some...
 * https://oeis.org/A253785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253785: composite numbers n = p₁⋯pₖ (p₁ ≤ … ≤ pₖ) that
   contain at least one adjacent pair of prime factors with
   pᵢ² < pᵢ₊₁ (indices 1…k‑1).  The function generates terms on
   demand and memoises them so that repeated calls are fast. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t n);   */
#include "../utils/factor_u64.h"        /* void factor_u64(uint64_t, uint64_t*,uint64_t*,size_t*); */

/* static state for memoisation */
static seq_t *cache           = NULL;   /* array of found terms           */
static size_t cache_len       = 0;      /* number of stored terms        */
static size_t cache_cap       = 0;      /* current array capacity        */
static uint64_t next_candidate = 4;     /* next integer to test (composite) */

/* Grow the cache array to accommodate at least `new_len` elements. */
static void cache_grow(size_t new_len)
{
    if (new_len <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < new_len)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function as required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)                       /* undefined for offset 1, harmless */
        return 0;

    /* Ensure we have at least n terms in the cache. */
    while (cache_len < n) {
        /* Increment the candidate until a new term is found. */
        while (true) {
            if (is_prime(next_candidate))
                ++next_candidate;     /* skip primes */
            else
                break;
        }

        /* Factor the composite candidate. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt;
        factor_u64(next_candidate, primes, exps, &cnt);

        /* Need at least two distinct prime factors. */
        bool ok = false;
        if (cnt >= 2) {
            for (size_t i = 0; i + 1 < cnt; ++i) {
                /* use 128‑bit to avoid overflow */
                if ((unsigned __int128)primes[i] * primes[i] < primes[i + 1]) {
                    ok = true;
                    break;
                }
            }
        }

        if (ok) {                    /* store term */
            cache_grow(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }

        ++next_candidate;  /* move past this candidate */
    }

    return cache[n - 1];
}
