/*
 * A344303  Number of cyclic subgroups of the group (C_n)^7, where C_n is the
 *          cyclic group of order n.
 * https://oeis.org/A344303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} (1 + (1 + p + … + p^6) * (1 + p^6 + … +
 *            p^(6(e‑1))))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 35, u64 n <= 1319,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 44 terms through n = 9567 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Contribution of a single prime power p^e. */
static seq_t prime_power_contrib(seq_t p, idx_t e)
{
    /* Sum 1 + p + … + p^6  */
    seq_t L1 = 1;
    seq_t powp = 1;
    for (idx_t i = 1; i <= 6; ++i) {
        powp *= p;
        L1 += powp;
    }

    /* p^6  */
    seq_t p6 = 1;
    for (idx_t i = 1; i <= 6; ++i)
        p6 *= p;

    /* Sum 1 + p^6 + … + p^(6(e‑1))  */
    seq_t L2 = 1;
    seq_t term = 1;
    for (idx_t k = 1; k < e; ++k) {
        term *= p6;
        L2 += term;
    }

    return 1 + L1 * L2;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p != 0) continue;
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        result *= prime_power_contrib((seq_t)p, e);
    }

    if (m > 1)  /* remaining prime factor with exponent 1 */
        result *= prime_power_contrib((seq_t)m, 1);

    return result;
}
