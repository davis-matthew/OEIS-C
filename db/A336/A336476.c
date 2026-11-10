/*
 * A336476  a(n) = gcd(A000593(n), A336475(n)).
 * https://oeis.org/A336476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A324121(A000265(n)) = gcd(m*d(m), sigma(m)), where m is
 *            the odd part of n
 * Range:     u8 n <= 44, u16 n <= 9064, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9065 (2026-11-08)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    while ((m & 1) == 0) m >>= 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);

    seq_t sig = 1;
    seq_t nd = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t term = 1, s = 1;
        for (uint64_t k = 0; k < exps[i]; ++k) {
            term = term * p;
            s = s + term;
        }
        sig = sig * s;
        nd = nd * (seq_t)(exps[i] + 1);
    }
    nd = nd * (seq_t)m;
    return gcd(nd, sig);
}
