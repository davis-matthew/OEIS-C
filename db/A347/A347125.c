/*
 * A347125  Möbius transform of A341529, sigma(n) * A003961(n).
 * https://oeis.org/A347125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} q^(e‑1) * (p^e*(q*p‑1) – q + 1)/(p‑1), * where
 *            q = nextprime(p)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 2416, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* factor p^e */
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }

            uint64_t q = next_prime(p);
            seq_t sq = (seq_t)q;

            /* compute p^e */
            seq_t pw_pe = (seq_t)1;
            for (uint64_t i = 0; i < e; ++i) pw_pe *= (seq_t)p;

            /* compute (p^e * (q*p-1) - q + 1) */
            seq_t qp_minus1 = sq * (seq_t)p - (seq_t)1;
            seq_t numerator = pw_pe * qp_minus1 - sq + (seq_t)1;

            /* divide by (p-1) */
            seq_t partial = numerator / (seq_t)(p - 1);

            /* multiply by q^(e-1) */
            seq_t qp_pow = (seq_t)1;
            for (uint64_t i = 1; i < e; ++i) qp_pow *= sq;

            seq_t contrib = partial * qp_pow;
            result *= contrib;
        }
    }

    /* leftover prime factor > sqrt(n) */
    if (m > 1) {
        uint64_t p = m;
        uint64_t q = next_prime(p);
        seq_t sq = (seq_t)q;

        seq_t pw_pe = (seq_t)p;
        seq_t qp_minus1 = sq * (seq_t)p - (seq_t)1;
        seq_t numerator = pw_pe * qp_minus1 - sq + (seq_t)1;
        seq_t partial = numerator / (seq_t)(p - 1);
        /* q^(1-1) = 1, so no extra multiplication */
        result *= partial;
    }

    return result;
}
