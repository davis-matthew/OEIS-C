/*
 * A295901  Unique sequence satisfying SumXOR_{d divides n} a(d) = n^2 for any
 *          n > 0, where SumXOR is the analog of summation under the binary
 *          XOR operation.
 * https://oeis.org/A295901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = \bop_{d|n,\,n/d\ \text{squarefree}} d^2 where \bop
 *            denotes XOR over all such divisors. This function computes the
 *            sequence a(n) for n >= 1
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 9810, u64 n <= 9810,
 *            u128 n <= 9810, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int _factor_distinct_primes(idx_t n, idx_t *primes, size_t *cnt)
{
    idx_t temp = n;
    size_t c = 0;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            primes[c++] = p;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)
        primes[c++] = temp;
    *cnt = c;
    return 0;
}

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by caller; behaviour for n < 1 is undefined. */
    /* Step 1: find the distinct prime factors of n. */
    idx_t pf[20];          /* 20 is more than enough for any 64‑bit integer. */
    size_t pf_cnt = 0;
    _factor_distinct_primes(n, pf, &pf_cnt);

    /* Step 2: enumerate all squarefree divisors m of n.
       For each such m, we include d = n/m in the XOR. */
    seq_t xor_sum = 0;
    size_t total_subsets = 1ULL << pf_cnt;   /* 2^pf_cnt */

    for (size_t mask = 0; mask < total_subsets; ++mask)
    {
        idx_t m_prod = 1;
        for (size_t j = 0; j < pf_cnt; ++j)
        {
            if (mask & (1ULL << j))
                m_prod *= pf[j];
        }
        idx_t d = n / m_prod;
        seq_t d_sq = (seq_t)d * (seq_t)d;
        xor_sum ^= d_sq;
    }

    return xor_sum;
}
