/*
 * A341835  Dirichlet g.f.: 1 / zeta(s)^9.
 * https://oeis.org/A341835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e‖n} (-1)^e * binomial(9, e)
 * Range:     i8 n <= 7, i16 n <= 719, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8973 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* compute coefficient for a prime power p^e */
static seq_t coeff_for_exp(idx_t e)
{
    if (e > 9) return 0;
    seq_t coeff = 1;
    for (idx_t k = 1; k <= e; ++k) {
        /* binomial(9, e) computed multiplicatively */
        coeff = coeff * ((seq_t)(9 - e + k));
        coeff = coeff / ((seq_t)k);
    }
    if (e & 1) coeff = -coeff;
    return coeff;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* sequence defined from offset 1 */
    seq_t prod = 1;
    uint64_t m = (uint64_t)n;

    /* trial‑division factorisation */
    for (uint64_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            idx_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            seq_t coeff = coeff_for_exp(e);
            prod = prod * coeff;
            if (prod == 0) return 0;    /* early exit if product becomes zero */
        }
    }
    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t coeff = coeff_for_exp(1);
        prod = prod * coeff;
    }
    return prod;
}
