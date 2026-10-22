/*
 * A383158  a(n) is the denominator of the mean of the maximum exponents in
 *          the prime factorizations of the divisors of n.
 * https://oeis.org/A383158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) is the denominator of the mean of the maximum exponents in
 *            the prime factorizations of the divisors of n. That is a(n) =
 *            τ(n) / gcd(∑_{d|n} A051903(d), τ(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dfs_max_exponents, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/dfs_max_exponents.h"

/*------------------------------------------------------------------*/
/*  a(n)  ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* factorize n: primes[], exps[] contain the distinct primes and
       corresponding exponents; cnt is the number of distinct primes. */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    /* tau(n) = ∏ (exp_i + 1) */
    seq_t tau = 1;
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(exps[i] + 1);

    /* Sum of maximum exponents over all divisors */
    seq_t sum = 0;
    if (cnt == 0) {          /* n = 1  →  only divisor 1, all exponents 0 */
        sum = 0;
    } else {
        dfs_max_exponents(0, 0, exps, cnt, &sum);
    }

    /* denominator = tau / gcd(sum, tau) */
    seq_t g = gcd(sum, tau);
    return tau / g;
}
