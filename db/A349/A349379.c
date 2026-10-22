/*
 * A349379  Möbius transform of A057521 (powerful part of n).
 * https://oeis.org/A349379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 0 if n has a prime factor of exponent 1; otherwise a(n) =
 *            ∏_{p^e‖n, e>1} (p^e - p^(e-1)) = ∏_{p^e‖n, e>1} p^(e-1) * (p-1)
 *            (with the special case e==2 giving p^2-1)
 * Range:     u8 n <= 288, u16 n <= 9886, u32 n <= 9886, u64 n <= 9886,
 *            u128 n <= 9886, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n = 1 => empty product = 1 */
    if (n == 0) return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (e == 1) {
            /* Any exponent 1 forces the product to zero */
            return (seq_t)0;
        }

        seq_t term;
        if (e == 2) {
            term = (seq_t)p * (seq_t)p - (seq_t)1;
        } else {
            /* Compute p^(e-1) */
            seq_t p_pow = (seq_t)1;
            for (uint64_t k = 1; k < e; ++k)
                p_pow *= (seq_t)p;

            term = p_pow * ((seq_t)p - (seq_t)1);
        }

        result *= term;
    }

    return result;
}
