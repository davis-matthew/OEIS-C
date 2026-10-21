/*
 * A282597  Expansion of phi_{14, 1}(x) where phi_{r, s}(x) = Sum_{n, m>0} m^r
 *          * n^s * x^{m*n}.
 * https://oeis.org/A282597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 23, u128 n <= 565,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A282597: Expansion of phi_{14,1}(x)
 * Formula: a(n) = n * σ₁₃(n)
 * Multiplicative: a(pⁿ) = pⁿ * σ₁₃(pⁿ)
 *            = pⁿ * (p^{13(n+1)} - 1) / (p¹³ - 1)
 */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"          /* helper for prime factorisation */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* a(0) = 0 */

    /* factorise n: primes[i] ^ exps[i]   (max. 64 factors for 64‑bit n) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;           /* multiplicative product */

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];    /* prime factor as seq_t */

        /* compute p^e  (needed for pⁿ in formula) */
        seq_t pe = (seq_t)1;
        for (uint64_t j = 0; j < exps[i]; ++j)
            pe *= p;

        /* compute p¹³  (base for σ₁₃(pⁿ) terms) */
        seq_t p13 = (seq_t)1;
        for (int j = 0; j < 13; ++j)
            p13 *= p;

        /* compute σ₁₃(pⁿ) = 1 + p¹³ + p²⁶ + … + p¹³ⁿ  */
        seq_t sigma = (seq_t)1;
        seq_t term   = (seq_t)1;
        for (uint64_t j = 1; j <= exps[i]; ++j)
        {
            term *= p13;    /* term = p^(13*j) */
            sigma += term;
        }

        /* local contribution: pⁿ * σ₁₃(pⁿ) */
        seq_t local = pe * sigma;

        /* multiply into running product */
        result *= local;
    }

    return result;                     /* a(n) = n * σ₁₃(n) via multiplicative rule */
}
