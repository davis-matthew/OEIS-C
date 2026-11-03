/*
 * A344221  a(n) = Sum_{k=1..n} tau(gcd(k,n)^3), where tau(n) is the number of
 *          divisors of n.
 * https://oeis.org/A344221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{k=1..n} tau(gcd(k,n)^3) Computed via
 *            multiplicativity: for a prime power p^e a(p^e) =
 *            (p^e*(p+2)-3)/(p-1)
 * Range:     u8 n <= 16, u16 n <= 2892, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9981 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t p_pow_e = ipow_u64_idx(p, e);
        seq_t num = p_pow_e * (seq_t)(p + 2) - (seq_t)3;
        seq_t term = num / (seq_t)(p - 1);
        result *= term;
    }
    return result;
}
