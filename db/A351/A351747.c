/*
 * A351747  a(n) = Sum_{p|n, p prime} n^lcm(p,n/p).
 * https://oeis.org/A351747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} n^lcm(p,n/p)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t nn = (uint64_t)n;
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t exponent = (exps[i] >= 2) ? (seq_t)(nn / p) : (seq_t)nn;
        sum += ipow((seq_t)nn, exponent);
    }
    return sum;
}
