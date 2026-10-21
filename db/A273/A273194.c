/*
 * A273194  a(n) = numerator(R(n,3)), where R(n,d) = (Product_{j prime to d}
 *          Pochhammer(j/d, n)) / n!.
 * https://oeis.org/A273194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( ( (1/3)_n * (2/3)_n ) / n! )
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   build_spf, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where (x)_n = x (x+1)...(x+n-1) is the Pochhammer symbol. */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_spf.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Prime tables: spf[i] = smallest prime factor of i (i == spf[i] iff i is prime). */
static idx_t   *spf          = NULL;
static idx_t   spf_len       = 0;
/* List of primes up to current limit, and mapping prime → index (in prime_list). */
static idx_t   *prime_list   = NULL;
static size_t  prime_cnt     = 0;
static int    *prime_index   = NULL;   /* prime_index[p] = position of p in prime_list    */
static size_t  prime_idx_len = 0;

/* Ensure prime tables cover all numbers up to limit. */
static void
ensure_prime_tables(idx_t limit)
{
    if (limit >= spf_len) {
        free(spf);
        build_spf(limit, &spf);          /* allocates a new array of size limit+1       */
        spf_len = limit + 1;
    }

    if (limit >= prime_idx_len) {
        /* Count primes up to the new limit. */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (spf[i] == i)
                ++cnt;

        /* Allocate new structures. */
        idx_t *new_primes = (idx_t *)malloc(cnt * sizeof(idx_t));
        int   *new_index  = (int  *)malloc((limit + 1) * sizeof(int));
        if (!new_primes || !new_index) {
            abort(); /* OOM – caller will fail gracefully. */
        }

        /* Initialise index map to -1. */
        for (idx_t i = 0; i <= limit; ++i)
            new_index[i] = -1;

        /* Fill primes and mapping. */
        size_t pos = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (spf[i] == i) {
                new_primes[pos]   = i;
                new_index[i]      = (int)pos;
                ++pos;
            }
        }

        free(prime_list);
        free(prime_index);
        prime_list   = new_primes;
        prime_index  = new_index;
        prime_cnt    = cnt;
        prime_idx_len = limit + 1;
    }
}

/* Legendre's formula: exponent of prime p in n!. */
static idx_t
legendre(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* To factor 3k+1 and 3k+2 up to 3(n-1)+2 = 3n+1, sieving up to 3n+2 is safe. */
    idx_t limit = 3 * n + 2;
    ensure_prime_tables(limit);

    /* Allocate exponent arrays for numerator and denominator. */
    idx_t *exp_num = (idx_t *)calloc(prime_cnt, sizeof(idx_t));
    idx_t *exp_den = (idx_t *)calloc(prime_cnt, sizeof(idx_t));
    if (!exp_num || !exp_den) {
        abort(); /* OOM – caller will fail gracefully. */
    }

    /* Accumulate exponents from the product ∏(3k+1)(3k+2). */
    for (idx_t k = 0; k < n; ++k) {
        idx_t m1 = 3 * k + 1;
        idx_t m2 = 3 * k + 2;

        /* Factor m1. */
        while (m1 > 1) {
            idx_t p   = spf[m1];
            idx_t idx = prime_index[p];
            while (m1 % p == 0) {          /* count multiplicity */
                ++exp_num[idx];
                m1 /= p;
            }
        }

        /* Factor m2. */
        while (m2 > 1) {
            idx_t p   = spf[m2];
            idx_t idx = prime_index[p];
            while (m2 % p == 0) {
                ++exp_num[idx];
                m2 /= p;
            }
        }
    }

    /* Denominator exponents: n! and the extra factor 3^(2n). */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        exp_den[i] = legendre(n, p);
        if (p == 3)
            exp_den[i] += 2 * n;      /* 3^(2n) contributes 2n to exponent of 3 */
    }

    /* Cancel common factors. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (exp_num[i] > exp_den[i])
            exp_num[i] -= exp_den[i];
        else
            exp_num[i] = 0;
    }

    /* Build the result from remaining prime powers. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (exp_num[i] == 0)
            continue;
        seq_t part = seq_pow((seq_t)prime_list[i], exp_num[i]);
        result = seq_mul(result, part);
    }

    free(exp_num);
    free(exp_den);
    return result;
}
