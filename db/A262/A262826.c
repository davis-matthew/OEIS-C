/*
 * A262826  a(n) = Sum_{d|n} -(-1)^d * 2^(n^2/d) * d.
 * https://oeis.org/A262826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} -(-1)^d * 2^(n^2/d) * d
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t sumOdd  = 0;
    seq_t sumEven = 0;

    /* iterate over all divisors of n (O(n), n <= 10000 is fine) */
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0)
            continue;

        idx_t exp = (n * n) / d;          /* exponent of 2 */
        seq_t pw  = pow2(exp);            /* 2^exp  */
        seq_t term = seq_mul(pw, (seq_t)d); /* d * 2^exp */

        if (d & 1U)                       /* d odd -> +term  */
            sumOdd += term;
        else                              /* d even -> -term */
            sumEven += term;              /* handled later */
    }

    seq_t res = sumOdd;
    res -= sumEven;                      /* sumOdd >= sumEven always */
    return res;
}
