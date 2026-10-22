/*
 * A389278  Primes p such that psi(p+1) = psi(p+2).
 * https://oeis.org/A389278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p such that Dedekind ψ(p+1) = ψ(p+2)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 26, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   append_candidate, factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"        /* factor_u64 */
#include "../utils/next_prime.h"       /* next_prime */
#include "../utils/append_candidate.h" /* append_candidate */

/* Dedekind psi function for a 64‑bit integer:  ψ(n) = n * ∏_{q|n}(1 + 1/q) */
static seq_t psi_uint64(uint64_t n)
{
    seq_t res = (seq_t)n;
    /* Temporary storage for prime factors. 64 is enough for any 64‑bit integer. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);          /* pr[0..cnt-1] contain distinct prime factors */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t q = (seq_t)pr[i];
        /* Multiply by (q + 1) / q.  The division is exact. */
        res = res * (q + (seq_t)1) / q;
    }
    return res;
}

/* Static cache of found primes. */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t last_prime_checked = 1;   /* start from 1 so the first call to next_prime gives 2 */

seq_t a(idx_t n)
{
    /* Ensure we have at least n primes cached. */
    while (cache_len < n) {
        uint64_t p = next_prime(last_prime_checked);
        if (p == 0) /* should not happen for 64‑bit primes */
            break;
        last_prime_checked = p;

        /* Compute ψ(p+1) and ψ(p+2). */
        seq_t psi1 = psi_uint64(p + 1);
        seq_t psi2 = psi_uint64(p + 2);

        if (psi1 == psi2) {
            append_candidate(&cache, &cache_len, &cache_cap, (seq_t)p);
        }
    }
    /* It is guaranteed that n >= 1 by the caller; the cache now
       has at least n elements, so index n-1 is safe. */
    return cache[n - 1];
}
