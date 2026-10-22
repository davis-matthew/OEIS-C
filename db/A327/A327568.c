/*
 * A327568  Exponent of the group GL(2, Z_n).
 * https://oeis.org/A327568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = LCM over prime powers p^e dividing n of ((p^2-1)*p^e) *
 *            using the multiplicative property: if gcd(m,n)=1 then
 *            a(mn)=lcm(a(m),a(n))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   factor_u64, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_gcd.h"
#include "../utils/seq_pow.h"

/* Exponent of GL(2, Z_n) */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n */
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p2minus1 = p * p - (seq_t)1;        /* (p^2 - 1) */
        seq_t p_pow = seq_pow(p, (idx_t)exps[i]);  /* p^e */
        seq_t val = p2minus1 * p_pow;             /* (p^2-1) * p^e */

        seq_t g = seq_gcd(result, val);
        result = (result / g) * val;
    }

    return result;
}
