/*
 * A323780  a(n) = denominator of Sum_{d|n} (tau(d)/sigma(d)) where tau(k) =
 *          the number of the divisors of k (A000005) and sigma(k) = the sum
 *          of the divisors of k (A000203).
 * https://oeis.org/A323780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of Sum_{d|n} (tau(d)/sigma(d))
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Small prime factor table */
static uint64_t *spf = NULL;     /* spf[i] = least prime factor of i */
static idx_t   spf_cap = 0;      /* capacity of spf array */

/* Ensure the spf table contains values up to limit */
static void ensure_spf(idx_t limit)
{
    if (spf_cap > limit)
        return;
    size_t new_cap = (size_t)limit + 1;
    uint64_t *new_spf = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    if (!new_spf)
        return; /* allocation failed – unlikely for limits up to 10000 */
    /* initialize spf values to themselves */
    for (size_t i = 0; i < new_cap; ++i)
        new_spf[i] = (uint64_t)i;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (new_spf[i] == i) { /* i is prime */
            for (size_t j = i * i; j <= limit; j += i)
                if (new_spf[j] == j)
                    new_spf[j] = i;
        }
    }
    if (spf)
        free(spf);
    spf = new_spf;
    spf_cap = new_cap;
}

/* Recursive divisor enumeration and rational addition */
static void dfs_divs(int pos, seq_t cur_tau, seq_t cur_sigma,
                     const uint64_t *primes, const uint8_t *exps, int k,
                     seq_t *num, seq_t *den)
{
    if (pos == k) {
        /* Add the fraction cur_tau / cur_sigma to the running sum */
        seq_t new_num = (*num) * cur_sigma + cur_tau * (*den);
        seq_t new_den = (*den) * cur_sigma;
        seq_t g = gcd(new_num, new_den);
        *num = new_num / g;
        *den = new_den / g;
        return;
    }

    uint64_t p = primes[pos];
    seq_t sigma_term = (seq_t)1; /* sigma(p^0) */
    seq_t p_pow      = (seq_t)1; /* p^0 */

    for (int f = 0; f <= exps[pos]; ++f) {
        seq_t tau_total   = cur_tau * ((seq_t)(f + 1));
        seq_t sigma_total = cur_sigma * sigma_term;
        dfs_divs(pos + 1, tau_total, sigma_total,
                 primes, exps, k, num, den);
        /* prepare for next exponent f+1 */
        p_pow *= (seq_t)p;
        sigma_term += p_pow;     /* sigma(p^{f+1}) = sigma(p^f) + p^{f+1} */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0; /* undefined for n<1 */

    /* Build spf table to factor n */
    ensure_spf(n);

    /* Factor n into primes and exponents */
    uint64_t primes_arr[64];
    uint8_t   exps_arr[64];
    int faccnt = 0;
    uint64_t temp = n;
    while (temp > 1) {
        uint64_t p = spf[temp];
        uint8_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        primes_arr[faccnt] = p;
        exps_arr[faccnt]   = e;
        ++faccnt;
    }

    /* Initialize sum = 0/1 */
    seq_t num = (seq_t)0;
    seq_t den = (seq_t)1;

    /* Enumerate all divisors and accumulate the sum */
    dfs_divs(0, (seq_t)1, (seq_t)1,
             primes_arr, exps_arr, faccnt, &num, &den);

    return den;
}
