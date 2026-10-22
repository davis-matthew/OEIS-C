/*
 * A341881  Number of ordered factorizations of n into 5 factors > 1.
 * https://oeis.org/A341881
 *
 * Offset:    32    (a(32) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered factorizations of n into 5 factors > 1.
 *            Implementation uses inclusion‑exclusion on prime exponent
 *            partitions
 * Range:     u8 n <= 479, u16 n <= 9923, u32 n <= 9923, u64 n <= 9923,
 *            u128 n <= 9923, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>

/* Binomial coefficient C(n,k), where n>=k and k <= 4, result fits in int64_t. */
static int64_t binom_small(int e, int k)
{
    /* Computes C(e + k, k) */
    int64_t res = 1;
    for (int i = 1; i <= k; ++i)
    {
        res = res * (int64_t)(e + k - (k - i));
        res = res / (int64_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Factorization of n into primes and exponents */
    uint64_t m = n;
    int exps[20];
    int cnt = 0;

    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }
    if (m > 1)
    {
        exps[cnt++] = 1;   /* remaining prime factor */
    }

    /* Combination coefficients C(5,s) for s = 0..5 */
    static const int C5s[6] = {1, 5, 10, 10, 5, 1};

    int64_t sum = 0;

    /* Inclusion–exclusion over subsets of factors that could be 1 */
    for (int s = 0; s <= 4; ++s)          /* s = number of factors forced to be 1 */
    {
        int64_t coeff = C5s[s];
        if (s & 1) coeff = -coeff;        /* (-1)^s */

        int k = 4 - s;                    /* remaining boxes */
        int64_t prod = 1;
        for (int i = 0; i < cnt; ++i)
        {
            if (k > 0)
            {
                prod *= binom_small(exps[i], k);
            }
            /* if k==0, factor contributes 1 */
        }
        sum += coeff * prod;
    }

    return (seq_t)sum;
}
