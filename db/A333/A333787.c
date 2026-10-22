/*
 * A333787  Fully multiplicative with a(2) = 2 and a(p) = p-1 for odd primes
 *          p.
 * https://oeis.org/A333787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^{v2(n)} * ∏_{p odd | n} (p-1)^{v_p(n)} (fully
 *            multiplicative)
 * Range:     u8 n <= 255, u16 n <= 8322, u32 n <= 8322, u64 n <= 8322,
 *            u128 n <= 8322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A333787 for given index n ≥ 1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;   /* work on 64‑bit integer */
    if (m <= 1) return (seq_t)1;   /* a(1) = 1 */

    seq_t result = 1;

    while (m > 1) {
        uint64_t p = 0;   /* current prime factor */
        int      e  = 0;   /* exponent of this prime */

        /* handle factor 2 separately */
        if (m % 2 == 0) {
            p = 2;
            while (m % 2 == 0) { m /= 2; ++e; }
        } else {
            /* find smallest odd prime factor */
            for (uint64_t i = 3; i <= m / i; i += 2) {
                if (m % i == 0) { p = i; break; }
            }
            if (p == 0) {        /* m is prime */
                p = m;
            }
            while (m % p == 0) { m /= p; ++e; }
        }

        /* compute p‑factor contribution */
        seq_t factor = 1;
        if (p == 2) {
            /* contribution is 2^e */
            for (int i = 0; i < e; ++i) factor *= (seq_t)2;
        } else {
            /* contribution is (p‑1)^e */
            seq_t base = (seq_t)(p - 1);
            for (int i = 0; i < e; ++i) factor *= base;
        }

        result *= factor;
    }

    return result;
}
