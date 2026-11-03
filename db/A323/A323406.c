/*
 * A323406  Greatest common divisor of Product (p_i^e_i)-1 and Product
 *          (p_i^e_i)+1, when n = Product (p_i^e_i): a(n) = gcd(A047994(n),
 *          A034448(n)).
 * https://oeis.org/A323406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(unitary_sigma(n), unitary_phi(n)) where
 *            unitary_sigma(n) = ∏(pᵉ + 1) and unitary_phi(n) = ∏(pᵉ – 1) over
 *            all prime powers pᵉ dividing n
 * Range:     u8 n <= 691, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9850 (2026-11-01)
 * Helpers:   factor_u64, gcd, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* For n = 1 the loops are skipped and we correctly return 1. */
    uint64_t uint_n = (uint64_t)n;          /* idx_t is an unsigned 64‑bit type */
    /* Arrays are large enough for all n <= 2⁶⁴ (max distinct primes < 20). */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t count = 0;

    factor_u64(uint_n, primes, exps, &count);

    seq_t sigma = 1;   /* product of (pᵉ + 1) */
    seq_t phi   = 1;   /* product of (pᵉ – 1) */

    for (size_t i = 0; i < count; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t e   = (seq_t)exps[i];
        seq_t p_pow = ipow(p, e);      /* pᵉ */
        seq_t p_pow_plus  = p_pow + 1;    /* pᵉ + 1 */
        seq_t p_pow_minus = p_pow - 1;    /* pᵉ – 1 */
        sigma = sigma * p_pow_plus;
        phi   = phi * p_pow_minus;
    }
    return gcd(sigma, phi);
}
