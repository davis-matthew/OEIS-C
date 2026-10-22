/*
 * A372881  a(n) = Sum_{j=1..n} Sum_{k=1..n} gcd(j*k,n).
 * https://oeis.org/A372881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{j=1..n} Sum_{k=1..n} gcd(j*k,n)
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 6299, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: multiplicative across prime powers using OEIS formula */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return result;
}

static seq_t prime_power_value(uint64_t p, uint64_t e)
{
    if (e == 0)
        return (seq_t)1;

    seq_t res = (seq_t)0;
    seq_t p_seq = (seq_t)p;
    for (uint64_t j = 0; j <= e; ++j)
    {
        seq_t f_j = (j == e) ? p_seq : (p_seq - (seq_t)1);
        for (uint64_t k = 0; k <= e; ++k)
        {
            seq_t f_k = (k == e) ? p_seq : (p_seq - (seq_t)1);

            uint64_t sum = j + k;
            uint64_t min_jk = (sum <= e) ? sum : e;
            int64_t exp = (int64_t)(2 * e + min_jk - j - k - 2);

            seq_t term;
            if (exp >= 0)
            {
                seq_t powp = pow_seq(p_seq, (idx_t)exp);
                term = seq_mul(seq_mul(f_j, f_k), powp);
            }
            else
            {
                /* The only negative exponent occurs for e==1, j==k==1;
                 * in this case f_j==f_k==p and term == p. */
                term = p_seq;
            }

            res += term;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        res = seq_mul(res, prime_power_value(primes[i], exps[i]));
    }
    return res;
}
