/*
 * A366572  a(n) = denominator(Bernoulli(n, x)) / denominator(Bernoulli''(n,
 *          x)).
 * https://oeis.org/A366572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator(Bernoulli(n, x)) / denominator(Bernoulli''(n,
 *            x))
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom_mod_lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include "../utils/binom_mod_lucas.h"

static int is_prime(int x)
{
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (int i = 3; i * i <= x; i += 2)
        if (x % i == 0) return 0;
    return 1;
}

static int mod_pow(int a, int e, int mod)
{
    long long res = 1;
    long long base = a % mod;
    while (e > 0)
    {
        if (e & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return (int)res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* generate all primes <= n+1 */
    int maxp = (int)(n + 1);
    int *prime_list = (int *)malloc((maxp + 1) * sizeof(int));
    int prime_cnt = 0;
    for (int p = 2; p <= maxp; ++p)
        if (is_prime(p))
            prime_list[prime_cnt++] = p;

    seq_t result = 1;

    for (int pi = 0; pi < prime_cnt; ++pi)
    {
        int p = prime_list[pi];
        int m = p - 1;           /* m = p-1 */
        if (m > n) continue;     /* no k <= n with p-1 | k */

        /* precompute factorial and inverse factorial modulo p */
        int *fact = (int *)malloc((p + 1) * sizeof(int));
        int *invfact = (int *)malloc((p + 1) * sizeof(int));
        fact[0] = 1;
        for (int i = 1; i < p; ++i)
            fact[i] = (int)((1LL * fact[i - 1] * i) % p);
        invfact[p - 1] = mod_pow(fact[p - 1], p - 2, p);
        for (int i = p - 2; i >= 0; --i)
            invfact[i] = (int)((1LL * invfact[i + 1] * (i + 1)) % p);

        int appearsNumerator = 0;
        int canceled = 0;

        for (int k = m; k <= (int)n; k += m)
        {
            int binom_mod = binom_mod_lucas((int)n, k, p, fact, invfact);
            if (!appearsNumerator && binom_mod % p != 0)
                appearsNumerator = 1;

            if (!canceled && k <= (int)n - 2)
            {
                int prod = (int)((1LL * binom_mod * ((n - k) % p)) % p);
                prod = (int)((1LL * prod * ((n - k - 1) % p)) % p);
                if (prod % p != 0)
                {
                    canceled = 1;
                    break;
                }
            }
        }

        free(fact);
        free(invfact);

        if (appearsNumerator && !canceled)
            result = result * (seq_t)p;
    }

    free(prime_list);
    return result;
}
