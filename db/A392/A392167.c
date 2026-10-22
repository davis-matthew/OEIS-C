/*
 * A392167  The Dedekind psi function value of the smallest number whose cube
 *          is divisible by n.
 * https://oeis.org/A392167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Dedekind psi of the smallest m such that m^3 is divisible
 *            by n. Multiplicative: a(p^e) = (p + 1) * p^floor((e - 1) / 3)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Since idx_t is unsigned 64‑bit, cast to uint64_t for local arithmetic. */
    uint64_t nn = (uint64_t)n;
    seq_t result = 1;

    uint64_t temp = nn;
    /* Trial‑division factorisation – fast enough for n up to ~10⁴ as in tests. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* aexp = ceil(e / 3) = (e + 2) / 3;  exponent for psi term is aexp - 1. */
            uint64_t aexp = (e + 2) / 3;
            uint64_t pow_exp = aexp - 1;   /* may be 0 */

            /* Compute p^(pow_exp) in seq_t */
            seq_t term = 1;
            for (uint64_t k = 0; k < pow_exp; ++k) {
                term *= (seq_t)p;
            }
            term *= (seq_t)(p + 1);   /* multiply by (p+1) */
            result *= term;
        }
    }

    /* Remaining factor, if any, is a prime > sqrt(nn). */
    if (temp > 1) {
        uint64_t p = temp;
        uint64_t e = 1;               /* exponent is 1 */
        uint64_t aexp = (e + 2) / 3;  /* equals 1 for e=1 */
        uint64_t pow_exp = aexp - 1;  /* equals 0 */

        seq_t term = 1;               /* p^0 == 1 */
        term *= (seq_t)(p + 1);
        result *= term;
    }

    return result;
}
