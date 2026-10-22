/*
 * A351600  a(n) = n^2 * Sum_{d^2|n} 1 / d^2.
 * https://oeis.org/A351600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^2 * Sum_{d^2 | n} 1/d^2 (equivalently
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 3072, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   a(p^e) = p^2 * (p^(2e) - p^(2*floor((e-1)/2))) / (p^2-1) ) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t pow_seq_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* handle 1 separately – its factorization is empty */
    if (n == 1) return 1;

    uint64_t N = (uint64_t)n;
    size_t cnt = 0;
    uint64_t primes[16];
    uint64_t exps[16];
    factor_u64(N, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];
        idx_t half = (e - 1) / 2;          /* floor((e-1)/2) */

        seq_t p2   = p * p;
        seq_t term1 = pow_seq_seq(p, 2 * e);          /* p^(2e)   */
        seq_t term2 = pow_seq_seq(p, 2 * half);       /* p^(2*half) */

        seq_t numerator = p2 * (term1 - term2);
        seq_t denominator = p2 - 1;
        seq_t a_pe = numerator / denominator;

        result *= a_pe;
    }

    return result;
}
