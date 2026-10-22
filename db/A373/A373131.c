/*
 * A373131  a(n) = Sum_{1 <= x_1, x_2, x_3 <= n} sigma( ( n/gcd(x_1, x_2, x_3,
 *          n) )^2 ).
 * https://oeis.org/A373131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} J_3(d) * sigma(d^2) (Jordan totient J_3)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 71, u64 n <= 5669,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_uint.h"

static void dfs_divisors(size_t idx, int primes_cnt,
                        const uint64_t *primes, const int *exp,
                        int *factors, seq_t *result)
{
    if (idx == primes_cnt) {
        /* compute J_3(d) and σ(d²) from exponent vector 'factors' */
        seq_t termJ = 1;
        seq_t termS = 1;
        for (size_t i = 0; i < primes_cnt; ++i) {
            int f = factors[i];
            uint64_t p = primes[i];
            if (f > 0) {
                /* J_3(p^f) = p^(3f) - p^(3f-3) */
                seq_t p3f = pow_uint(p, 3 * f);
                seq_t p3   = pow_uint(p, 3);
                seq_t p3f_minus = p3f / p3;
                seq_t jprime = p3f - p3f_minus;
                termJ *= jprime;

                /* σ(p^(2f)) = (p^(2f+1)-1)/(p-1) */
                seq_t sigma_prime = (pow_uint(p, 2 * f + 1) - 1) / (p - 1);
                termS *= sigma_prime;
            }
        }
        *result += termJ * termS;
        return;
    }

    /* iterate exponent for prime idx */
    const uint64_t p = primes[idx];
    const int max_f = exp[idx];
    for (int f = 0; f <= max_f; ++f) {
        factors[idx] = f;
        dfs_divisors(idx + 1, primes_cnt, primes, exp, factors, result);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* safety for out of offset */

    /* Factorize n by trial division (fast enough for n up to 10⁴, works for larger too) */
    uint64_t primes[20];
    int exp[20];
    size_t primes_cnt = 0;
    idx_t rem = n;
    for (uint64_t p = 2; p <= rem / p; ) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) { rem /= p; ++e; }
            primes[primes_cnt] = p;
            exp[primes_cnt]    = e;
            ++primes_cnt;
        }
        if (p == 2) p = 3;
        else        p += 2;
    }
    if (rem > 1) {
        primes[primes_cnt] = rem;
        exp[primes_cnt]    = 1;
        ++primes_cnt;
    }

    int factors[20] = {0};
    seq_t sum = 0;
    dfs_divisors(0, primes_cnt, primes, exp, factors, &sum);
    return sum;
}
