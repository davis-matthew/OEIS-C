/*
 * A276240  a(n) = denominator of rational fraction of function Gamma[5/4]^2
 *          Gamma[n + 3/4]^2/(Gamma[3/4]^2 Gamma[n + 5/4]^2).
 * https://oeis.org/A276240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 14, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A276240
    a(n) = denominator^2 of the reduced fraction
           Gamma[5/4]^2 Gamma[n+3/4]^2 / (Gamma[3/4]^2 Gamma[n+5/4]^2)
    Equivalently: a(n) = ∏_{p odd ≤ 4n+1} p^(max(0, vp(den) – vp(num)))^2
    where the exponent difference vp(den)-vp(num) is computed from the
    factors of all odd numbers ≤4n+1.  All terms are odd squares.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */

/* static workspace for spf, primes and exponents */
static uint32_t *spf          = NULL;     /* smallest prime factor for each i ≤ limit */
static int      *prime_index  = NULL;     /* prime_index[p] = index of prime p in primes */
static uint32_t *primes       = NULL;     /* list of primes up to current limit */
static int       *diff_exp     = NULL;     /* diff_exponent per prime (den - num)   */
static idx_t     max_limit     = 0;        /* maximum limit built so far          */
static size_t    prime_count   = 0;        /* number of primes < = limit           */

/* Build spf and primes up to new_limit (>= max_limit) */
static void build_tables(idx_t new_limit)
{
    if (new_limit <= max_limit) return;

    /* free previous arrays */
    free(spf);          free(prime_index); free(primes); free(diff_exp);

    /* allocate arrays for the new limit (+1 for 0/1 indices) */
    size_t sz = (size_t)new_limit + 1;
    spf          = (uint32_t *)calloc(sz, sizeof(uint32_t));   /* initialized to 0 */
    prime_index  = (int *)     calloc(sz, sizeof(int));
    if (!spf || !prime_index) abort();

    /* Simple sieve for spf */
    for (idx_t i = 2; i <= new_limit; ++i)
    {
        if (spf[(size_t)i] == 0)           /* i is prime */
        {
            spf[(size_t)i] = (uint32_t)i;
            /* mark multiples */
            if ((idx_t)i * i <= new_limit)  /* avoid overflow */
            {
                for (idx_t j = i * i; j <= new_limit; j += i)
                    if (spf[(size_t)j] == 0)
                        spf[(size_t)j] = (uint32_t)i;
            }
        }
    }

    /* build primes list */
    primes = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!primes) abort();
    prime_count = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (spf[(size_t)i] == (uint32_t)i)
            primes[prime_count++] = (uint32_t)i;

    /* prime_index map */
    for (size_t i = 0; i < sz; ++i) prime_index[(size_t)i] = -1;
    for (size_t i = 0; i < prime_count; ++i)
        prime_index[(size_t)primes[i]] = (int)i;

    /* diff array – same size as primes but allocate up to limit for safety */
    diff_exp = (int *)calloc(sz, sizeof(int));
    if (!diff_exp) abort();

    max_limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* maximum odd number to consider */
    idx_t limit = 4 * n + 1;
    build_tables(limit);

    /* reset exponent differences */
    /* only prime indices up to prime_count are used; clear them */
    for (size_t i = 0; i < prime_count; ++i)
        diff_exp[i] = 0;

    /* factor all odd k ≤ limit, updating diff_exponent */
    for (idx_t k = 1; k <= limit; k += 2)
    {
        /* skip 1: it has no prime factors */
        if (k == 1) continue;

        int sign = (k % 4 == 1) ? +1 : -1;   /* + for k≡1 mod4, - for k≡3 mod4 */
        idx_t x = k;

        while (x > 1)
        {
            uint32_t p = spf[(size_t)x];
            int cnt = 0;
            while (x % p == 0)
            {
                x /= p;
                ++cnt;
            }
            int idx = prime_index[(size_t)p];
            diff_exp[idx] += sign * cnt;
        }
    }

    /* compute the final product: product over primes with positive diff
       of prime^(2*diff) */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i)
    {
        int d = diff_exp[i];
        if (d > 0)
        {
            idx_t exp = (idx_t)(2 * d);
            seq_t pow_val = big_pow((seq_t)primes[i], (seq_t)exp);
            result = seq_mul(result, pow_val);
        }
    }
    return result;
}
