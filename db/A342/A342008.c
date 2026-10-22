/*
 * A342008  Numbers k such that Euler totient phi(k) is a multiple of the
 *          arithmetic derivative of k.
 * https://oeis.org/A342008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A342008: Numbers k such that Euler totient φ(k) is a multiple of the arithmetic derivative of k.
   This implementation tests successive integers, computes their arithmetic derivative and Euler totient via the
   prime factorizations obtained from factor_u64, and records those k for which φ(k) is a multiple of the derivative.
   Results are memoised so that a(n) can be requested in any order.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h" /* void factor_u64(uint64_t n,uint64_t *pr,uint64_t *ex,size_t *cnt); */

static seq_t *cache = NULL;          /* cached values of the sequence */
static size_t cache_len = 0;         /* number of cached values */
static uint64_t next_candidate = 2;  /* next integer to test */

/* Ensure the cache can hold at least 'need' values. */
static void ensure_cache_cap(size_t need)
{
    if (cache_len >= need)
        return;
    size_t new_cap = cache_len ? cache_len * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failure – abort program as per build conventions */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;
}

/* Compute Euler totient φ(k) from its prime factorisation. */
static uint64_t phi_from_factors(uint64_t k, const uint64_t *pr, const uint64_t *ex, size_t cnt)
{
    uint64_t phi = k;
    for (size_t i = 0; i < cnt; ++i) {
        phi -= phi / pr[i];
    }
    return phi;
}

/* Return the nth term (1‑based) of A342008. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset protection */

    /* Return cached value if already known. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Temporary arrays for factorization; enough for numbers <= 2^64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        uint64_t k = next_candidate;

        /* Factorise k. */
        factor_u64(k, primes, exps, &cnt);

        /* Arithmetic derivative: Σ e_i * (k / p_i). */
        uint64_t d = 0;
        for (size_t i = 0; i < cnt; ++i) {
            d += exps[i] * (k / primes[i]);
        }

        /* Euler totient. */
        uint64_t phi = phi_from_factors(k, primes, exps, cnt);

        /* Check condition φ(k) mod d == 0. */
        if (d != 0 && (phi % d) == 0) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
