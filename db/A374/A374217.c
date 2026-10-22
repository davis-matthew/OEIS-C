/*
 * A374217  The 3-adic valuation of sopfr(n), where sopfr is the sum of prime
 *          factors with repetition, A001414.
 * https://oeis.org/A374217
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v_3( sopfr(n) ) where sopfr(n) = sum of prime factors of
 *            n with multiplicity
 * Range:     u8 n <= 6825, u16 n <= 9832, u32 n <= 9832, u64 n <= 9832,
 *            u128 n <= 9832, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; behaviour below the offset is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* Factor n into primes and exponents. */
    /* We use a small fixed-size buffer; for n up to 64‑bit this is sufficient. */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute sopfr(n) = Σ p_i × e_i */
    seq_t sopfr = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sopfr += (seq_t)primes[i] * (seq_t)exps[i];
    }

    /* Compute the exponent of 3 in sopfr(n). */
    seq_t v = 0;
    while (sopfr % (seq_t)3 == 0) {
        ++v;
        sopfr /= (seq_t)3;
    }

    return v;
}
