/*
 * A364932  a(n) = phi(psi(n)).
 * https://oeis.org/A364932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = φ(ψ(n)) (Euler's totient of Dedekind's ψ)
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Computes a(n) = φ(ψ(n)) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)              /* out of range, not used by OEIS */
        return (seq_t)0;

    /* Factorise n: n = ∏ p^e */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute ψ(n) = ∏ p^(e‑1) * (p+1) */
    uint64_t psi = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* p^(e‑1) */
        uint64_t p_pow = 1;
        for (uint64_t k = 1; k < e; ++k)
            p_pow *= p;

        psi *= p_pow;              /* multiply by p^(e‑1)   */
        psi *= (p + 1);            /* then by (p+1)        */
    }

    /* Compute φ(ψ) */
    /* Factorise ψ */
    size_t cnt2 = 0;
    factor_u64(psi, primes, exps, &cnt2);

    uint64_t phi = psi;
    for (size_t i = 0; i < cnt2; ++i)
    {
        uint64_t p = primes[i];
        phi = phi / p * (p - 1);   /* φ = φ / p * (p‑1)   */
    }

    return (seq_t)phi;
}
