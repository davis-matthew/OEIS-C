/*
 * A371418  The largest aliquot divisor of the sum of divisors of n; a(1) = 1.
 * https://oeis.org/A371418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest proper divisor of σ(n) (σ(n) = sum of all
 *            positive divisors of n). Equivalent implementation: a(n) = σ(n)
 *            / spf(σ(n)), where spf(m) is the smallest prime factor of m (for
 *            m = 1, spf(1) is defined as 1)
 * Range:     u8 n <= 16, u16 n <= 1730, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Largest aliquot divisor of the sum of divisors of n. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by definition; safety check. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* ---------- compute σ(n) ---------- */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    uint64_t nn = (uint64_t)n;

    factor_u64(nn, primes, exps, &cnt);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        seq_t   pe  = 1;                 /* p^(e+1) */
        for (uint64_t j = 0; j < e + 1; ++j)
            pe *= (seq_t)p;
        seq_t term = (pe - 1) / (seq_t)(p - 1);
        sigma *= term;
    }

    /* ---------- largest proper divisor of σ(n) ---------- */
    if (sigma <= 1)
        return (seq_t)1;   /* σ(1) = 1 → a(1) = 1 */

    /* Find smallest prime divisor of sigma by trial division. */
    uint64_t sigma_ll = (uint64_t)sigma;   /* σ(n) fits in 64‑bit for all tested n. */
    uint64_t spf = sigma_ll;              /* assume sigma is prime */
    for (uint64_t d = 2; (unsigned __int128)d * d <= sigma_ll; ++d) {
        if (sigma_ll % d == 0) {
            spf = d;
            break;
        }
    }

    return sigma / (seq_t)spf;
}
