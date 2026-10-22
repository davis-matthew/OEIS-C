/*
 * A327569  Exponent of the group SL(2, Z_n).
 * https://oeis.org/A327569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the exponent of SL(2, Z_n). The function is
 *            multiplicative: a(1) = 1. For an odd prime p and exponent e ≥ 1:
 *            a(p^e) = (p^2 – 1) · p^e / 2. For p = 2: a(2^e) = 3 · 2^e. For
 *            coprime m, n: a(m·n) = lcm(a(m), a(n))
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    /* Factor n.  The helper stores primes and exponents in arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];
        seq_t val;

        if (p == (seq_t)2) {
            /* a(2^e) = 3 * 2^e. */
            val = (seq_t)3 * pow_uint((uint64_t)2, e);
        } else {
            /* a(p^e) = ((p^2 - 1) * p^e) / 2. */
            seq_t square = p * p;
            seq_t term   = square - (seq_t)1;
            seq_t power  = pow_uint((uint64_t)p, e);
            val = (term * power) / (seq_t)2;
        }

        seq_t g = gcd(result, val);
        result = (result / g) * val;
    }

    return result;
}
