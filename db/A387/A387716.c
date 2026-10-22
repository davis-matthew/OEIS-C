/*
 * A387716  Greatest divisor d of n such that sigma(d) < 2*d < A003961(d), or
 *          1 if no such divisor exists, where A003961 is fully multiplicative
 *          with a(p) = nextprime(p).
 * https://oeis.org/A387716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest divisor d of n such that sigma(d) < 2*d <
 *            A003961(d), or 1 if no such divisor exists
 * Range:     u8 n <= 62, u16 n <= 5315, u32 n <= 8424, u64 n <= 8424,
 *            u128 n <= 8424, big any n
 * Helpers:   factor_u64, next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"

static void dfs(size_t idx, seq_t d, seq_t sigma, seq_t a,
                uint64_t *primes, size_t *exps, size_t cnt,
                idx_t *best)
{
    if (idx == cnt) {
        seq_t twice = d * 2;
        if (sigma < twice && twice < a && d > *best) {
            *best = (idx_t)d;
        }
        return;
    }

    uint64_t p = primes[idx];
    size_t emax = exps[idx];
    uint64_t nextp = next_prime(p);                /* nextprime(p) */

    /* exponent 0 case */
    dfs(idx + 1, d, sigma, a, primes, exps, cnt, best);

    /* exponent 1 .. emax */
    for (size_t e = 1; e <= emax; ++e) {
        seq_t p_pow      = pow_uint(p, e);
        seq_t sigma_part = (pow_uint(p, e + 1) - 1) / (p - 1);
        seq_t a_part     = pow_uint(nextp, e);
        seq_t d_next     = d * p_pow;
        seq_t sigma_next = sigma * sigma_part;
        seq_t a_next     = a * a_part;
        dfs(idx + 1, d_next, sigma_next, a_next,
            primes, exps, cnt, best);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 1;   /* outside offset – undefined, return 1 */

    uint64_t primes[16];
    size_t   exps[16];
    size_t   cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    idx_t best = 1;
    dfs(0, (seq_t)1, (seq_t)1, (seq_t)1,
        primes, exps, cnt, &best);

    return (seq_t)best;
}
