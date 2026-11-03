/*
 * A372381  The number of divisors of the largest divisor of n whose number of
 *          divisors is a power of 2.
 * https://oeis.org/A372381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} 2^floor(log2(e+1)) (multiplicative)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9544 (2026-11-01)
 * Helpers:   factor_u64, ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ilog2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1, but safety */
    if (n == 1) return 1;

    uint64_t nn = (uint64_t)n;   /* n fits in 64 bits for the provided helper */

    /* temporary storage for prime factors and exponents */
    uint64_t pr[64], ex[64];
    size_t   cnt = 0;

    factor_u64(nn, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e   = ex[i];
        unsigned t   = ilog2(e + 1);      /* floor(log2(e+1)) */
        seq_t   mul  = ((seq_t)1 << t);              /* 2^t */
        res *= mul;
    }
    return res;
}
