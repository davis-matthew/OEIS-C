/*
 * A341857  a(n) = psi(n^2)/n, psi = A002322.
 * https://oeis.org/A341857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 996, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = ψ(n²)/n where ψ is Carmichael λ function
 *   For n = ∏ pᵉ, let n² = ∏ p^(2e).
 *   For each p:
 *     if p == 2:
 *         λ(2)   = 1, λ(4)   = 2,
 *         λ(2ᵏ) = 2^(k‑2) for k ≥ 3
 *     else:
 *         λ(pᵏ) = p^(k‑1)·(p‑1)
 *   Then ψ(n²) = lcm { λ(p^(2e)) }.
 *   Finally a(n) = ψ(n²) / n.   (Division is exact.)
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)       /* offset is 1, but guard for 0 */
        return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;

    /* factor n; primes[], exps[], cnt are filled */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t lcm_val = 1;          /* ψ(n²) accumulator, start at 1 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];
        seq_t val;

        if (p == 2) {
            if (e == 1)
                val = 2;                                       /* λ(4)  */
            else
                val = pow_uint(2, (idx_t)(2 * (e - 1)));       /* λ(2^(2e)) = 2^(2e-2) */
        } else {
            /* λ(p^(2e)) = p^(2e-1) · (p-1) */
            seq_t t = pow_uint(p, (idx_t)(2 * e - 1));
            val = t * (seq_t)(p - 1);
        }

        /* lcm(lcm_val, val) = lcm_val / gcd(lcm_val,val) * val */
        seq_t g = gcd(lcm_val, val);
        seq_t l_div_g = lcm_val / g;
        lcm_val = l_div_g * val;
    }

    /* a(n) = ψ(n²) / n, exact division */
    return lcm_val / (seq_t)n;
}
