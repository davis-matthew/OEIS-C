/*
 * A372679  a(n) = phi(15 * n)/8.
 * https://oeis.org/A372679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = phi(15 * n) / 8 Multiplicative form: a(p^e) = p^e if p ==
 *            3 or 5, otherwise a(p^e) = (p-1) * p^(e-1)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        seq_t   p_seqt = (seq_t)p;

        /* compute the contribution of this prime power */
        if (p == 3 || p == 5) {
            seq_t pow = 1;
            for (uint64_t k = 0; k < e; ++k)
                pow *= p_seqt;
            result *= pow;
        } else {
            seq_t pow = 1;            /* p^(e-1) */
            for (uint64_t k = 1; k < e; ++k)
                pow *= p_seqt;
            result *= ((seq_t)(p - 1)) * pow;
        }
    }

    return result;
}
