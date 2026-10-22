/*
 * A342672  a(n) = lcm(sigma(n), A003961(n)), where A003961 is fully
 *          multiplicative with a(prime(k)) = prime(k+1), and sigma is the sum
 *          of divisors of n.
 * https://oeis.org/A342672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm( sigma(n), A003961(n) ) * where A003961 is fully
 *            multiplicative and A003961(p_k) = p_{k+1}
 * Range:     u8 n <= 8, u16 n <= 63, u32 n <= 4095, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static inline int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return 0;
    return 1;
}

/* Return the next prime after prime p (p is guaranteed to be prime). */
static inline uint64_t next_prime(uint64_t p)
{
    if (p < 2) return 2;
    uint64_t cand = p + 1;
    if (cand % 2 == 0) cand += 1;
    for (;;)
    {
        if (is_prime_uint64(cand)) return cand;
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;               /* by definition a(1) = 1 */

    /* Fast trial division factorization */
    uint64_t num = (uint64_t)n;
    const int MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    int exps[MAX_FACTORS];
    int cnt = 0;

    for (uint64_t p = 2; p * p <= num; p += (p == 2 ? 1 : 2))
    {
        if (num % p == 0)
        {
            int e = 0;
            while (num % p == 0)
            {
                num /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (num > 1)
    {
        primes[cnt] = num;
        exps[cnt] = 1;
        ++cnt;
    }

    seq_t sigma = 1;          /* sigma(n) */
    seq_t mul_map = 1;        /* A003961(n) */

    for (int i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        int e = exps[i];

        /* Compute term for sigma: (p^(e+1) - 1)/(p-1) */
        seq_t p_pow = 1;
        for (int k = 0; k < e + 1; ++k) p_pow *= (seq_t)p;
        seq_t term_sigma = (p_pow - 1) / ((seq_t)p - 1);
        sigma *= term_sigma;

        /* Compute A003961 term: q^e where q = next prime after p */
        uint64_t q = next_prime(p);
        seq_t q_pow = 1;
        for (int k = 0; k < e; ++k) q_pow *= (seq_t)q;
        mul_map *= q_pow;
    }

    /* lcm(sigma, mul_map) = sigma / gcd(sigma, mul_map) * mul_map */
    seq_t g = gcd(sigma, mul_map);
    return (sigma / g) * mul_map;
}
