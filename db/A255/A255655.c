/*
 * A255655  The sum of the odd terms in row n of A050873.
 * https://oeis.org/A255655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e‖n} f(p^e) * where f(2^e) = 2^{e-1} (e>0) * and
 *            f(p^e) = ((p-1)*e + p) * p^{e-1} for odd primes p
 * Range:     u8 n <= 62, u16 n <= 4724, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, undefined for n=0 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            seq_t mult = 1;
            for (uint64_t j = 0; j < e - 1; ++j) mult *= (seq_t)2;
            result *= mult;
        } else {
            seq_t p_pow = 1;
            for (uint64_t j = 0; j < e - 1; ++j) p_pow *= (seq_t)p;
            seq_t coeff = ((seq_t)(p - 1) * (seq_t)e) + (seq_t)p;
            result *= coeff * p_pow;
        }
    }
    return result;
}
