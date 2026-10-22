/*
 * A364082  Expansion of Sum_{k>0} k * x^(3*k-2) / (1 - x^(4*k-3)).
 * https://oeis.org/A364082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Sum_{d | (4*n-1), d==1 (mod 4)} (d+3)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 4*n - 1 (value fits in idx_t for all intended n) */
    idx_t m = 4 * n - 1;

    /* Factorise m by trial division, store factors and exponents */
    idx_t p, temp = m;
    idx_t primes[20];
    idx_t exps[20];
    int pf_cnt = 0;

    /* factor 2 separately */
    if (temp % 2 == 0)
    {
        idx_t e = 0;
        while (temp % 2 == 0)
        {
            temp /= 2;
            ++e;
        }
        primes[pf_cnt] = 2;
        exps[pf_cnt++] = e;
    }

    /* odd factors */
    p = 3;
    /* use idx_t sqrt via loop condition */
    for (; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt++] = e;
        }
    }
    if (temp > 1)
    {
        primes[pf_cnt] = temp;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Generate all divisors of m */
    idx_t divs[2048];               /* safe upper bound for small m */
    int div_cnt = 1;
    divs[0] = 1;

    for (int i = 0; i < pf_cnt; ++i)
    {
        idx_t prime = primes[i];
        idx_t pow = 1;
        int old_cnt = div_cnt;
        for (int e = 1; e <= exps[i]; ++e)
        {
            pow *= prime;
            for (int j = 0; j < old_cnt; ++j)
            {
                if (div_cnt < 2048)
                    divs[div_cnt++] = divs[j] * pow;
            }
        }
    }

    /* Sum (d+3) over divisors d ≡ 1 (mod 4) and divide by 4 */
    seq_t sum = 0;
    for (int i = 0; i < div_cnt; ++i)
    {
        idx_t d = divs[i];
        if ((d & 3) == 1)          /* d % 4 == 1 */
        {
            sum += (seq_t)d + 3;
        }
    }
    return sum / 4;
}
