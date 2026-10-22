/*
 * A327095  Expansion of Sum_{k>=1} k * x^k * (1 - x^k + x^(2*k)) / (1 -
 *          x^(4*k)).
 * https://oeis.org/A327095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏ₚⁿ a(pᵉ) where a(2ᵉ) = 2^(e−1) (e ≥ 1), and a(pᵉ) =
 *            (p^(e+1) − 1)/(p − 1) for odd primes p
 * Range:     u8 n <= 16, u16 n <= 586, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* a(1) = 1, nothing else needs special handling */
    if (n == 1) return (seq_t)1;

    idx_t temp = n;
    idx_t v2   = 0;

    /* factor out powers of 2 */
    while ((temp & 1ULL) == 0) {
        ++v2;
        temp >>= 1;
    }

    seq_t result = (seq_t)1;

    if (v2 > 0)
        result *= pow2(v2 - 1);             /* a(2ᵉ) = 2^(e−1) */

    /* factor the remaining odd part */
    idx_t odd = temp;
    for (idx_t p = 3; p * p <= odd; p += 2) {
        if (odd % p == 0) {
            idx_t e = 0;
            while (odd % p == 0) {
                odd /= p;
                ++e;
            }
            seq_t p_pow   = pow_seq((seq_t)p, (int)(e + 1)); /* p^(e+1) */
            seq_t factor  = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
            result *= factor;
        }
    }

    if (odd > 1) {    /* remaining prime factor */
        seq_t p = (seq_t)odd;
        seq_t factor = (pow_seq(p, 2) - (seq_t)1) / ((seq_t)odd - (seq_t)1);
        result *= factor;
    }

    return result;
}
