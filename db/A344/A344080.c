/*
 * A344080  a(n) = Sum_{d|n} tau(d)^n, where tau(n) is the number of divisors
 *          of n.
 * https://oeis.org/A344080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} tau(d)^n, where tau(k) is the number of
 *            divisors of k
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Recursive helper: enumerate all combinations of exponent choices for
   prime factors.  tau_prod holds the product \prod (exponent_i + 1)
   for the divisor built so far. */
static void dfs_divisor_sum(size_t pos, size_t pcnt, idx_t exps[],
                             seq_t tau_prod, idx_t n, seq_t *sum)
{
    if (pos == pcnt)
    {
        *sum = *sum + big_pow(tau_prod, (seq_t)n);
        return;
    }
    for (idx_t e = 0; e <= exps[pos]; ++e)
    {
        seq_t new_tau = tau_prod * (e + 1);
        dfs_divisor_sum(pos + 1, pcnt, exps, new_tau, n, sum);
    }
}

seq_t a(idx_t n)
{
    /* Factor n into distinct primes and exponents. */
    idx_t exps[20];
    size_t pcnt = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (m > 1)
    {
        exps[pcnt] = 1;
        ++pcnt;
    }

    seq_t sum = 0;
    dfs_divisor_sum(0, pcnt, exps, (seq_t)1, n, &sum);
    return sum;
}
