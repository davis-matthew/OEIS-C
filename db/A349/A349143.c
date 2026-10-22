/*
 * A349143  a(n) = Sum_{d|n} A038040(d) * A348507(n/d), where A038040(n) =
 *          n*tau(n), A348507(n) = A003959(n) - n, and A003959 is fully
 *          multiplicative with a(p) = (p+1).
 * https://oeis.org/A349143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 287, u32 n <= 9031, u64 n <= 9031,
 *            u128 n <= 9031, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = Sum_{d|n} A038040(d) * A348507(n/d)
    A038040(d) = d * tau(d)
    A348507(m) = A003959(m) - m
    A003959 is multiplicative with a(p) = p + 1
    Thus A003959(m) = Π_{p^k||m} (p+1)^k
    The implementation factors n once, generates all divisors,
    and evaluates the sum exactly in seq_t. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64 */

static void _a_rec(uint64_t *pr, uint64_t *exp, size_t cnt, size_t idx,
                  uint64_t *cur, seq_t *sum, seq_t n_seq)
{
    if (idx == cnt) {
        /* leaf: cur holds the exponent choice for each prime */
        seq_t d = 1;
        seq_t tau = 1;
        seq_t comp_prod = 1;   /* product of primes^(exp - cur) = n/d   */
        seq_t prod_add1 = 1;   /* product of (p+1)^(exp - cur)          */

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t e = cur[i];
            uint64_t comp = exp[i] - e;

            /* p^e */
            seq_t pow_p = 1;
            for (uint64_t k = 0; k < e; ++k) pow_p *= (seq_t)pr[i];
            d *= pow_p;
            tau *= (seq_t)(e + 1);

            /* p^comp for n/d */
            seq_t pow_comp = 1;
            for (uint64_t k = 0; k < comp; ++k) pow_comp *= (seq_t)pr[i];
            comp_prod *= pow_comp;

            /* (p+1)^comp for A003959(n/d) */
            seq_t pow_p1 = 1;
            for (uint64_t k = 0; k < comp; ++k) pow_p1 *= (seq_t)(pr[i] + 1);
            prod_add1 *= pow_p1;
        }

        seq_t inner = prod_add1 - comp_prod; /* A348507(n/d) */
        *sum += d * tau * inner;
        return;
    }

    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        cur[idx] = e;
        _a_rec(pr, exp, cnt, idx + 1, cur, sum, n_seq);
    }
}

seq_t a(idx_t n)
{
    /* a(1) is zero, but the recursion also yields this value */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    if (n == 0) return (seq_t)0;          /* safety for n==0 if ever called */

    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t cur[64] = {0};   /* current exponents in recursion */
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;

    _a_rec(primes, exps, cnt, 0, cur, &sum, n_seq);

    return sum;
}
