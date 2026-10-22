/*
 * A345264  a(n) = Sum_{d|n} rad(d) * mu(n/d)^2.
 * https://oeis.org/A345264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} rad(d) * mu(n/d)^2
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Multiplicative: for prime power p^e,
    a(p)   = p + 1
    a(p^e) = 2*p  for e >= 2 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;  /* rad(1)=1 and only d=1 works */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term;
        if (exps[i] == 1) {
            term = (seq_t)primes[i] + (seq_t)1;
        } else {
            term = (seq_t)2 * (seq_t)primes[i];
        }
        res = res * term;
    }
    return res;
}
