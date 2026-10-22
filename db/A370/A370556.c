/*
 * A370556  a(n) is the denominator of f(n)*conj(f(n)), where f(n) =
 *          Product_{k=1..n} (1/k + i), i is the imaginary unit, and conj(z)
 *          is the complex conjugate of z.
 * https://oeis.org/A370556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of ∏_{k=1..n} ((1/k)+i)((1/k)-i) after
 *            reducing the fraction to lowest terms. The value is the product
 *            of primes p≤n raised to the exponent max(0, 2·v_p(n!) –
 *            v_p(∏_{k=1..n}(1+k²)) )
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"

static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* offset starts at 1 */

    /* generate all primes ≤ n */
    uint64_t *prime_list = NULL;
    size_t prime_cnt = 0;
    build_primes_upto((idx_t)n, &prime_list, &prime_cnt);

    /* map each prime to its array index for quick lookup */
    int *prime_index = (int *)malloc((size_t)(n + 1) * sizeof(int));
    if (!prime_list || !prime_index)
    {
        free(prime_list);
        return 0;
    }
    for (size_t i = 0; i <= n; ++i)
        prime_index[i] = -1;
    for (size_t i = 0; i < prime_cnt; ++i)
        prime_index[prime_list[i]] = (int)i;

    /* exponent of each prime in ∏(1+k²) */
    idx_t *exp_num = (idx_t *)calloc(prime_cnt, sizeof(idx_t));
    if (!exp_num)
    {
        free(prime_list);
        free(prime_index);
        return 0;
    }

    for (idx_t k = 1; k <= n; ++k)
    {
        uint64_t t = (uint64_t)k * (uint64_t)k + 1;
        uint64_t tmp = t;

        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = prime_list[i];
            if (p * p > tmp) break;

            if (tmp % p == 0)
            {
                int cnt = 0;
                while (tmp % p == 0)
                {
                    tmp /= p;
                    ++cnt;
                }
                exp_num[i] += cnt;
            }
        }

        if (tmp > 1)
        {
            /* tmp is prime or product of primes > sqrt(tmp);
               the only possible prime factor not yet counted is a prime ≤ n.
               If it is such a prime, increment its exponent.  */
            if (tmp <= n && tmp <= (uint64_t)n)
            {
                int idx = prime_index[(size_t)tmp];
                if (idx >= 0)
                    exp_num[idx] += 1;
            }
        }
    }

    /* compute the reduced denominator */
    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];

        /* exponent of p in n! (Legendre's formula) */
        idx_t e_fact = 0;
        idx_t div = n;
        while (div)
        {
            div /= p;
            e_fact += div;
        }
        idx_t e_den = e_fact * 2;         /* exponent in (n!)² */

        idx_t e_numer = exp_num[i];
        idx_t diff = (e_den > e_numer) ? (e_den - e_numer) : 0;

        if (diff)
            result *= pow_seq_t((seq_t)p, diff);
    }

    free(prime_list);
    free(prime_index);
    free(exp_num);
    return result;
}
