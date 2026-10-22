/*
 * A344465  a(n) = Sum_{d|n} d^(mu(d)^2).
 * https://oeis.org/A344465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = σ(rad(n)) + τ(n) – 2^ω(n) (sum of squarefree divisors of
 *            n)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle n=0 gracefully (not part of OEIS offset) */
    if (n == 0)
        return 0;

    uint64_t nn = (uint64_t)n;

    /* Temporary storage for prime factorization */
    static uint64_t primes[64];
    static uint64_t exps[64];
    static size_t cnt;

    factor_u64(nn, primes, exps, &cnt);

    seq_t rad = 1;          /* product of distinct primes */
    seq_t sigma_rad = 1;    /* σ(rad(n)) */
    seq_t tau = 1;          /* τ(n) = ∏ (exp+1) */
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        rad *= p;
        sigma_rad *= (1 + p);
        tau *= (exps[i] + 1);
    }

    /* 2^ω(n) where ω(n) = cnt */
    seq_t pow2omega = (seq_t)1 << cnt;

    return sigma_rad + tau - pow2omega;
}
