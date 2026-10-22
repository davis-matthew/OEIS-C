/*
 * A380164  a(n) is the value of the Euler totient function when applied to
 *          the largest unitary divisor of n that is a square.
 * https://oeis.org/A380164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Euler totient of the largest unitary square divisor of n
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 1;          /* a(1) = 1, and defensive case for 0 */

    /* The maximum number of prime factors for a 64‑bit integer is 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n into primes and their exponents. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        /* Contribution only when the exponent is even. */
        if ((exps[i] & 1ULL) == 0ULL) {
            /* Compute p^(e-1) by repeated multiplication. */
            seq_t pow = 1;
            for (uint64_t k = 0; k < exps[i] - 1; ++k) {
                pow = pow * (seq_t)primes[i];
            }
            /* (p‑1) * p^(e‑1)  */
            seq_t term = ((seq_t)primes[i] - 1) * pow;
            result = result * term;
        }
    }

    return result;
}
