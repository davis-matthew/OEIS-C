/*
 * A280184  Number of cyclic subgroups of the group C_n x C_n x C_n x C_n,
 *          where C_n is the cyclic group of order n.
 * https://oeis.org/A280184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Product_{p^e || n} [ 1 + (p^3+p^2+p+1) *
 *            (p^(3e)-1)/(p^3-1) ]
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 1019, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicativity is used: a(mn) = a(m)a(n) when gcd(m,n)=1. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

static seq_t pow_seq_uint(seq_t base, idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= base;
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;          /* a(1)=1, a(0) unused */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t ans = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];

        /* Compute s = p^3 + p^2 + p + 1 */
        seq_t p2 = p * p;
        seq_t p3 = p2 * p;
        seq_t s = p3 + p2 + p + 1;

        /* Compute numerator = p^(3e) - 1 */
        seq_t power = pow_seq_uint(p, 3 * e);
        seq_t numerator = power - 1;

        /* Compute denominator = p^3 - 1 */
        seq_t denom = p3 - 1;

        /* ratio = numerator / denom (exact division) */
        seq_t ratio = numerator / denom;

        /* term = 1 + s * ratio */
        seq_t term = 1 + s * ratio;

        ans *= term;
    }

    return ans;
}
