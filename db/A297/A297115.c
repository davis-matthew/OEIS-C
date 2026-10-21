/*
 * A297115  Möbius transform of A000120, binary weight of n.
 * https://oeis.org/A297115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} μ(n/d) * popcount(d) where μ is the Möbius
 *            function. This implementation factors n, enumerates all divisors
 *            d, computes μ(n/d) on the fly during the enumeration, and sums
 *            μ(n/d)*popcount(d). It uses only integer arithmetic and works
 *            for any seq_t/idx_t defined by the build system
 * Range:     i8 n <= 9801, i16 n <= 9801, i32 n <= 9801, i64 n <= 9801,
 *            i128 n <= 9801, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Recursive helper: enumerate divisors of n and accumulate the Möbius
   transform sum.  mu_cur holds μ(n/d_current) for the currently
   constructed part of the divisor. */
static void dfs(const uint64_t *primes, const int *exps, int num_primes,
                int pos, uint64_t cur_d, int mu_cur, seq_t *sum)
{
    if (pos == num_primes) {
        /* leaf: complete divisor cur_d is built */
        seq_t weight = (seq_t)__builtin_popcountll(cur_d);
        if (mu_cur != 0)
            *sum += (seq_t)mu_cur * weight;
        return;
    }

    const uint64_t p   = primes[pos];
    const int     e_total = exps[pos];
    uint64_t p_pow = 1;  /* p^e_div */

    for (int e_div = 0; e_div <= e_total; ++e_div) {
        int e_rest = e_total - e_div;         /* exponent in n/d */
        int mu_part;                          /* contribution to μ(n/d) from this prime */
        if (e_rest == 0)
            mu_part = 1;
        else if (e_rest == 1)
            mu_part = -1;
        else
            mu_part = 0;

        int new_mu = mu_cur * mu_part;
        if (new_mu != 0) {
            dfs(primes, exps, num_primes, pos + 1,
                cur_d * p_pow, new_mu, sum);
        }

        p_pow *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* special case: n >= 1 by definition of the sequence */
    if (n == 1)
        return (seq_t)1;   /* a(1) = popcount(1) * μ(1) = 1 */

    /* factorise n */
    uint64_t tmp = n;
    uint64_t primes[64];
    int      exps[64];
    int      num_primes = 0;

    /* factor 2 */
    int e2 = 0;
    while ((tmp & 1ULL) == 0ULL) {
        ++e2;
        tmp >>= 1;
    }
    if (e2 > 0) {
        primes[num_primes] = 2ULL;
        exps[num_primes]   = e2;
        ++num_primes;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            primes[num_primes] = p;
            exps[num_primes]   = e;
            ++num_primes;
        }
    }

    if (tmp > 1) {          /* remaining prime factor */
        primes[num_primes] = tmp;
        exps[num_primes]   = 1;
        ++num_primes;
    }

    /* enumerate all divisors and accumulate the sum */
    seq_t sum = 0;
    dfs(primes, exps, num_primes, 0, 1ULL, 1, &sum);

    return sum;
}
