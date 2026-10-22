/*
 * A365336  The sum of exponentially odd divisors of the square root of the
 *          largest square dividing n.
 * https://oeis.org/A365336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = A033634(A000188(n)) = Sum of the exponentially odd
 *            divisors of sqrt(largest square dividing n). Computed as product
 *            over prime powers in the sqrt‑square part: For each prime p^e in
 *            n, let β = floor(e/2); if β>0 then S(p,β) = 1 + p * (p^(β +
 *            (β&1)) – 1) / (p^2 – 1). The answer is the produc...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined, but harmless */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t beta = e >> 1;                 /* floor(e/2) */
        if (beta == 0) continue;                /* prime absent in the sqrt part */

        seq_t p_val = (seq_t)p;
        seq_t numerator, denom, div, s;

        /* Exponent in the geometric‑series formula */
        size_t exp = beta + (beta & 1ULL);      /* beta if even, beta+1 if odd */

        numerator = pow_seq(p_val, (int)exp) - 1;
        denom     = p_val * p_val - 1;
        div       = numerator / denom;          /* exact division */

        s = 1 + p_val * div;                    /* contribution of this prime */

        result *= s;
    }

    return result;
}
