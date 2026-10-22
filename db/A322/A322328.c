/*
 * A322328  a(n) = A005361(n) * 4^A001221(n) for n > 0.
 * https://oeis.org/A322328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative, a(p^e) = 4*e (for all primes p and exponents
 *            e>0)
 * Range:     u8 n <= 179, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Thus a(n) = product over prime powers of (4 * exponent). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)1;

    uint64_t x = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(x, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        res *= (seq_t)(4ULL * exps[i]);
    }
    return res;
}
