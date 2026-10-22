/*
 * A327268  Dirichlet g.f.: 1 / (zeta(s) * zeta(s-1) * (1 - 2^(2 - s))).
 * https://oeis.org/A327268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Dirichlet inverse of the Liouville function λ(n). a(1) =
 *            1; a(2) = 1; a(2^e) = 3·2^(2e-3) (e≥2); For an odd prime p: a(p)
 *            = -(p+1), a(p^2) = p, a(p^e) = 0 (e≥3)
 * Range:     i8 n <= 31, i16 n <= 511, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* Handle powers of 2. */
    uint64_t temp = (uint64_t)n;
    uint64_t e2 = 0;
    while ((temp & 1) == 0) {
        ++e2;
        temp >>= 1;
    }
    if (e2 > 0) {
        if (e2 == 1) {
            /* a(2) = 1 – no change to result. */
        } else {
            /* a(2^e) = 3 * 2^(2e-3) */
            seq_t pow_val = pow2((idx_t)(2 * e2 - 3));
            seq_t term = (seq_t)3 * pow_val;
            result *= term;
        }
    }

    /* Factor the remaining odd part. */
    uint64_t m = temp;
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                ++e;
                m /= p;
            }
            if (e == 1) {
                seq_t p_seq = (seq_t)p;
                result *= -(p_seq + (seq_t)1);
            } else if (e == 2) {
                result *= (seq_t)p;
            } else { /* e ≥ 3 for odd prime → a(n)=0 */
                return (seq_t)0;
            }
        }
    }
    if (m > 1) { /* remaining prime factor with exponent 1 */
        seq_t p_seq = (seq_t)m;
        result *= -(p_seq + (seq_t)1);
    }

    return result;
}
