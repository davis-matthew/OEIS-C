/*
 * A327790  Decompose the multiplicative group of integers modulo n as a
 *          product of cyclic groups C_{k_1} X C_{k_2} X ... X C_{k_r}, where
 *          k_i > 1, k_i divides k_j for i < j; then a(n) = Product_{i=1..r}
 *          phi(k_i), phi = A000010.
 * https://oeis.org/A327790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of φ(kᵢ) where (ℤ/nℤ)× ≅ C_{k₁} × C_{k₂} × … ×
 *            C_{kᵣ} with kᵢ > 1 and kᵢ | kⱼ for i < j
 * Range:     u8 n <= 556, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the invariant‑factor decomposition of the
   multiplicative group modulo n by factoring n, determining the cyclic
   components of each prime‑power factor, breaking those components into
   prime‑power cyclic groups, and multiplying the Euler totient values
   of the final invariant factors.  The result fits into the available
   seq_t type for the requested build. */

#include "../utils/factor_u64.h"
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n into prime powers */
    factor_u64(nn, primes, exps, &cnt);

    seq_t ans = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2)
        {
            /* (ℤ/2ᵉℤ)× structure */
            if (e == 1)
                continue;                     /* trivial group */
            else if (e == 2)
                ;                              /* one component C₂ → φ(2)=1 */
            else
            {
                /* two components C₂ and C_{2^{e-2}}; φ(2)=1, φ(2^{e-2})=2^{e-3} */
                ans *= (seq_t)1 << (e - 3);
            }
        }
        else
        {
            /* Compute φ(pᵉ) = p^{e-1}(p-1) */
            uint64_t pow_p_le = 1;
            for (uint64_t k = 1; k < e; ++k)       /* p^(e-1) */
                pow_p_le *= p;
            uint64_t phi_pe = pow_p_le * (p - 1);

            /* Factor φ(pᵉ) into prime powers */
            uint64_t fp[64];
            uint64_t fexp[64];
            size_t fcnt = 0;
            factor_u64(phi_pe, fp, fexp, &fcnt);

            for (size_t j = 0; j < fcnt; ++j)
            {
                uint64_t q = fp[j];
                uint64_t t = fexp[j];

                /* φ(qᵗ) = q^{t-1} (q-1) */
                seq_t pow_q_tm1 = (seq_t)1;
                for (uint64_t k = 0; k + 1 < t; ++k)   /* multiply (t-1) times */
                    pow_q_tm1 *= (seq_t)q;

                seq_t phi_qt = pow_q_tm1 * (seq_t)(q - 1);
                ans *= phi_qt;
            }
        }
    }

    return ans;
}
