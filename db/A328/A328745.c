/*
 * A328745  Dirichlet g.f.: Product_{p prime} 1 / (1 - p^(-s))^p.
 * https://oeis.org/A328745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^k‖n} binomial(p + k − 1, k)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS A328745. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1, and 0 is never requested */

    /* Factorize n.  64‑bit numbers have at most 64 prime factors (more or
       less rarely), so 64 slots are plenty for our purposes. */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = binomial_small((idx_t)primes[i] + (idx_t)exps[i] - 1,
                                    (idx_t)exps[i]);
        result *= term;
    }
    return result;
}
