/*
 * A363904  Expansion of Sum_{k>0} x^(3*k) / (1 - x^(4*k))^2.
 * https://oeis.org/A363904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4) * Sum_{d|n, d%4==3} (d+1)
 * Range:     u8 n <= 506, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Utility: compute smallest prime factor array up to a given limit.
   The array is reallocated if a larger limit is required. */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

static void ensure_spf(idx_t n)
{
    if (n <= spf_limit)
        return;

    free(spf);
    spf = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!spf)
        exit(1);

    for (idx_t i = 0; i <= n; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (i * i <= n) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }
    spf_limit = n;
}

/* Recursive divisor enumeration. */
static void enumerate_divisors(size_t idx,
                               idx_t prod,
                               const idx_t *primes,
                               const idx_t *exp,
                               size_t fac_cnt,
                               seq_t *sum)
{
    if (idx == fac_cnt) {
        if ((prod & 3ULL) == 3ULL)          /* prod % 4 == 3 */
            *sum += (seq_t)(prod + 1) / 4;
        return;
    }

    idx_t p = primes[idx];
    idx_t m = 1;
    for (idx_t e = 0; e <= exp[idx]; ++e) {
        enumerate_divisors(idx + 1, prod * m, primes, exp, fac_cnt, sum);
        m *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_spf(n);

    /* factorize n */
    idx_t primes[64];
    idx_t exp[64];
    size_t fac_cnt = 0;

    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes[fac_cnt] = p;
        exp[fac_cnt] = e;
        ++fac_cnt;
    }

    seq_t sum = 0;
    enumerate_divisors(0, 1, primes, exp, fac_cnt, &sum);
    return sum;
}
