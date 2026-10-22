/*
 * A334795  a(n) = Product_{d|n} lcm(d, tau(d)) where tau(k) is the number of
 *          divisors of k (A000005).
 * https://oeis.org/A334795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ∏_{d|n} lcm(d, τ(d)) where τ(k) is the divisor count of k
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 47, u64 n <= 119, u128 n <= 419,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* gcd for arbitrary-precision seq_t (uses operator %) */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* lcm using the local gcd */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    if (a == (seq_t)0 || b == (seq_t)0)
        return (seq_t)0;
    return (a / gcd_seq(a, b)) * b;
}

/* Trial‑division factorisation of n, into arrays pr[] and ex[].
   cnt will contain the number of distinct prime factors. */
static void factor_uint64(uint64_t n, uint64_t pr[], uint64_t ex[], size_t *cnt)
{
    *cnt = 0;
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            uint64_t e = 0;
            while (t % p == 0)
            {
                t /= p;
                e++;
            }
            pr[*cnt] = p;
            ex[*cnt] = e;
            (*cnt)++;
        }
    }
    if (t > 1)
    {
        pr[*cnt] = t;
        ex[*cnt] = 1;
        (*cnt)++;
    }
}

/* Recursive enumeration of all divisors, computing the product of lcm(d, τ(d)). */
static void dfs_divisors(const uint64_t pr[], const uint64_t ex[],
                         size_t pos, size_t k, seq_t cur_d, seq_t cur_tau,
                         seq_t *res)
{
    if (pos == k)
    {
        seq_t l = lcm_seq(cur_d, cur_tau);
        *res = (*res) * l;
        return;
    }
    uint64_t p = pr[pos];
    seq_t pow = (seq_t)1; /* p^e */
    for (uint64_t e = 0; e <= ex[pos]; ++e)
    {
        seq_t new_d = cur_d * pow;
        seq_t new_tau = cur_tau * (e + 1);
        dfs_divisors(pr, ex, pos + 1, k, new_d, new_tau, res);
        pow = pow * (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* undefined below offset; safe fallback */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    factor_uint64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    dfs_divisors(primes, exps, 0, cnt, (seq_t)1, (seq_t)1, &result);
    return result;
}
