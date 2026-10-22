/*
 * A381267  a(n) = numerator( [(x*y*z*u)^n] 1/sqrt(1 - (x + y + z + u*(y +
 *          z))) ).
 * https://oeis.org/A381267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( [(x*y*z*u)^n] 1/sqrt(1 - (x + y + z + u*(y +
 *            z))) )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* exponent of prime p in n! */
static idx_t exp_in_factorial(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n)
    {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* upper bound for all numbers that appear in the products */
    const idx_t limit = 6 * n;
    /* smallest prime factor array */
    idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!spf) return (seq_t)0;
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = i;

    /* linear sieve: set spf for composite numbers */
    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (spf[i] == i)
        {
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }

    /* collect primes and build index map */
    idx_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (spf[i] == i) ++prime_cnt;

    idx_t *primes = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!primes) { free(spf); return (seq_t)0; }

    /* map prime value -> position in primes[] */
    int *prime_idx = (int *)calloc((limit + 1), sizeof(int));
    if (!prime_idx) { free(primes); free(spf); return (seq_t)0; }

    idx_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == i)
        {
            primes[idx] = i;
            prime_idx[i] = (int)idx;
            ++idx;
        }
    }

    /* exponent accumulator for each prime */
    int *exp_arr = (int *)calloc(prime_cnt, sizeof(int));
    if (!exp_arr) { free(prime_idx); free(primes); free(spf); return (seq_t)0; }

    /* --- factor (1/2)_n^2: product of odd numbers (1+2k) twice --- */
    for (idx_t k = 0; k < n; ++k)
    {
        idx_t m = 1 + 2 * k;
        while (m > 1)
        {
            idx_t p = spf[m];
            int cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                ++cnt;
            }
            exp_arr[prime_idx[p]] += 2 * cnt;      /* square */
        }
    }

    /* --- factor (1/6)_n: product of numbers 1+6k --- */
    for (idx_t k = 0; k < n; ++k)
    {
        idx_t m = 1 + 6 * k;
        while (m > 1)
        {
            idx_t p = spf[m];
            int cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                ++cnt;
            }
            exp_arr[prime_idx[p]] += cnt;
        }
    }

    /* --- factor (5/6)_n: product of numbers 5+6k --- */
    for (idx_t k = 0; k < n; ++k)
    {
        idx_t m = 5 + 6 * k;
        while (m > 1)
        {
            idx_t p = spf[m];
            int cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                ++cnt;
            }
            exp_arr[prime_idx[p]] += cnt;
        }
    }

    /* --- factor 108^n (108 = 2^2 * 3^3) --- */
    exp_arr[prime_idx[2]] += (int)(2 * n);
    exp_arr[prime_idx[3]] += (int)(3 * n);

    /* --- subtract denominator contributions from rising factorials --- */
    exp_arr[prime_idx[2]] -= (int)(4 * n);   /* 2^{4n} from (1/2)_n^2 & (1/6)_n^2 */
    exp_arr[prime_idx[3]] -= (int)(2 * n);   /* 3^{2n} from (1/6)_n & (5/6)_n */

    /* --- subtract factorial part (n!)^4 ---> exponent is 4 * exp_in_factorial --- */
    for (idx_t i = 0; i < prime_cnt && primes[i] <= n; ++i)
    {
        idx_t p = primes[i];
        idx_t e = exp_in_factorial(n, p);
        exp_arr[i] -= (int)(4 * e);
    }

    /* build result from remaining positive exponents */
    seq_t result = (seq_t)1;

    for (idx_t i = 0; i < prime_cnt; ++i)
    {
        int e = exp_arr[i];
        if (e > 0)
        {
            seq_t powv = seq_pow(primes[i], (idx_t)e);
            result = seq_mul(result, powv);
        }
    }

    /* free temporary storage */
    free(exp_arr);
    free(prime_idx);
    free(primes);
    free(spf);

    return result;
}
