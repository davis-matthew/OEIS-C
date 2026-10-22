/*
 * A318467  a(n) = 2*n XOR A000203(n), where XOR is bitwise-xor (A003987) and
 *          A000203 = sum of divisors.
 * https://oeis.org/A318467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*n XOR σ(n) (σ(n) = sum of all positive divisors of n)
 * Range:     u8 n <= 16, u16 n <= 1366, u32 n <= 9837, u64 n <= 9837,
 *            u128 n <= 9837, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation below keeps a cache of σ values up to
   the maximum n requested so far.  Each new value is computed by
   prime‑factorising the integer, using the supplied factor_u64
   helper, and then applying the divisor‑sum formula
   ∏ (p^(e+1)-1) / (p-1).  The result is returned in type seq_t.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow_seq.h"      /* seq_t pow_seq(seq_t base, unsigned int exp) */

/* Cache for σ(n).  sigma_cache[0] is unused. */
static seq_t *sigma_cache = NULL;
static size_t cache_cap = 1;      /* current number of allocated slots */

/* Compute the divisor sum σ(n) for a single n. */
static seq_t sigma_of(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)primes[i];
        unsigned int exp = (unsigned int)(exps[i] + 1);
        seq_t p_pow = pow_seq(p, exp);                 /* p^(e+1) */
        seq_t term  = (p_pow - (seq_t)1) / (p - (seq_t)1); /* (p^(e+1)-1)/(p-1) */
        result *= term;
    }
    return result;
}

/* Ensure the sigma cache contains values up to at least n. */
static void ensure_sigma_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;

    size_t new_cap = (size_t)n + 1;          /* include index n */
    seq_t *tmp = (seq_t *)realloc(sigma_cache, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort();                /* out of memory */
    sigma_cache = tmp;

    /* Set unused entry 0 */
    if (cache_cap == 0) sigma_cache[0] = (seq_t)0;

    /* Compute missing σ values */
    for (size_t i = cache_cap; i <= (size_t)n; ++i)
        sigma_cache[i] = sigma_of((idx_t)i);

    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* offset 1, undefined for 0 */

    ensure_sigma_cache(n);

    seq_t two_n = ((seq_t)n << 1);          /* 2 * n */
    seq_t sigma = sigma_cache[n];
    return two_n ^ sigma;
}
