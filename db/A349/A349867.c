/*
 * A349867  Generalized multiplicative Euler phi function of order 2 (see El
 *          Hindi et al. link).
 * https://oeis.org/A349867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   For n ≥ 1, let the prime factorisation be n = 2^e2 × ∏_{p>2}
 *            p^e(p). Then a(n) = a(2^e2) × ∏_{p>2} a(p^e(p)), where a(2^e2) =
 *            1 if e2 < 4, 2^{e2-1} otherwise; a(p^e) = φ(p-1) if e = 1;
 *            a(p^e) = φ(p-1) × (p^{e-1} – p^{e-2}) if e > 1; Here φ denotes
 *            Euler’s totient function, which is computed for t...
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi_uint.h"

static seq_t pow_uint(idx_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= (seq_t)base;
    }
    return result;
}

static seq_t pow2(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result *= 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;

    idx_t m = n;            /* local copy for factorisation */
    seq_t result = 1;

    /* Handle factor 2 separately */
    idx_t e2 = 0;
    while ((m & 1) == 0) {
        ++e2;
        m >>= 1;
    }
    if (e2 > 0) {
        if (e2 < 4) {
            result *= 1;
        } else {
            result *= pow2(e2 - 1);
        }
    }

    /* Factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            seq_t part;
            if (e == 1) {
                part = phi_uint(p - 1);
            } else {
                seq_t phi_p_pow = pow_uint(p, e - 1) - pow_uint(p, e - 2);
                seq_t phi_p_minus1 = phi_uint(p - 1);
                part = phi_p_pow * phi_p_minus1;
            }
            result *= part;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        seq_t part = phi_uint(m - 1);
        result *= part;
    }

    return result;
}
