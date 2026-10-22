/*
 * A319446  Exponent of the group of the Eisenstein integers in a reduced
 *          system modulo n.
 * https://oeis.org/A319446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = exponent of (ℤ[ω]/nℤ[ω])*,
 * where ω = (1+√−3)/2.  For n = ∏ pᵉ:
 *   if p = 3:       a(3ᵉ) = 6          (e=1), 2·3⁽ᵉ⁻¹⁾ (e≥2)
 *   if p ≡ 1 mod 3: a(pᵉ) = (p−1)·p⁽ᵉ⁻¹⁾
 *   if p ≡ 2 mod 3: a(pᵉ) = (p²−1)·p⁽ᵉ⁻¹⁾
 * and for coprime factors a(mn)=lcm(a(m),a(n)).
 * Thus a(n) = lcm over all prime‑power components.  The function
 * implements this computation exactly.
 */

/* Required utilities */
#include <stdint.h>
#include "../utils/factor_u64.h"  /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp) */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b) */

/* Compute A319446(n) */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t pe_minus1 = pow_uint(p, (idx_t)(e - 1));
        seq_t local;

        if (p == 3) {
            if (e == 1)
                local = (seq_t)6;
            else
                local = (seq_t)2 * pe_minus1;
        } else if (p % 3 == 1) {
            local = (seq_t)(p - 1) * pe_minus1;
        } else { /* p % 3 == 2 */
            local = (seq_t)((p * p) - 1) * pe_minus1;
        }

        /* lcm(result, local) */
        seq_t g = gcd(result, local);
        result = (result / g) * local;
    }

    return result;
}
