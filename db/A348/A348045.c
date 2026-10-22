/*
 * A348045  Möbius transform of A252463, where A252463 shifts the prime
 *          factorization of odd numbers one step towards smaller primes and
 *          divides even numbers by two.
 * https://oeis.org/A348045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n} A008683(n/d) * A252463(d) (μ is A008683)
 * Range:     u8 n <= 262, u16 n <= 9364, u32 n <= 9364, u64 n <= 9364,
 *            u128 n <= 9364, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include "../utils/build_spf.h"

/* External sequence type */
seq_t a(idx_t n);

/* Allocate and free from the build's malloc (C/C++ compatible) */
static idx_t *spf = NULL;          /* smallest prime factor */
static int   *mu  = NULL;          /* Möbius function values */
static seq_t *A252 = NULL;         /* A252463 values up to current N */
static idx_t *prevPrime = NULL;    /* prevPrime[p] for each prime p <= N */
static idx_t *primes = NULL;       /* list of primes up to N */
static size_t prime_cnt = 0;       /* number of primes stored */
static idx_t  max_prepared = 0;    /* highest N for which tables are built */

/* Ensure all tables exist up to N */
static void prepare(idx_t N)
{
    if (N <= max_prepared)
        return;                     /* already available */

    /* Free old tables if any */
    free(spf);
    free(mu);
    free(A252);
    free(prevPrime);
    free(primes);
    spf        = NULL;
    mu         = NULL;
    A252       = NULL;
    prevPrime  = NULL;
    primes     = NULL;
    prime_cnt  = 0;
    max_prepared = N;

    /* 1. Build smallest prime factor array */
    build_spf(N, &spf);            /* spf[0] unused, spf[1] unused */

    /* 2. Build list of primes */
    size_t prime_cap = 64;
    primes = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == i) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                primes = (idx_t *)realloc(primes, prime_cap * sizeof(idx_t));
            }
            primes[prime_cnt++] = i;
        }
    }

    /* 3. Allocate previous–prime table */
    prevPrime = (idx_t *)calloc((size_t)(N + 1), sizeof(idx_t));
    if (prime_cnt > 0) {
        prevPrime[2] = 2;          /* dummy for prime 2 (unused) */
        for (size_t k = 1; k < prime_cnt; ++k)
            prevPrime[primes[k]] = primes[k-1];
    }

    /* 4. Compute Möbius function for 1..N */
    mu = (int *)malloc((size_t)(N + 1) * sizeof(int));
    mu[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        idx_t p   = spf[i];
        idx_t m   = i / p;
        if (m % p == 0)
            mu[i] = 0;
        else
            mu[i] = -mu[m];
    }

    /* 5. Compute A252463 values for 1..N */
    A252 = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
    A252[1] = 1;            /* by definition */
    for (idx_t num = 2; num <= N; ++num) {
        if ((num & 1) == 0) {                 /* even */
            A252[num] = (seq_t)(num / 2);
        } else {                              /* odd */
            seq_t prod = 1;
            idx_t m = num;
            while (m > 1) {
                idx_t p = spf[m];
                int e = 0;
                while (m % p == 0) {
                    m /= p;
                    ++e;
                }
                seq_t pp = (seq_t)prevPrime[p];
                seq_t pow = 1;
                for (int k = 0; k < e; ++k)
                    pow *= pp;
                prod *= pow;
            }
            A252[num] = prod;
        }
    }
}

/* The required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1, result irrelevant */

    prepare(n);            /* make sure tables exist up to n */

    int64_t sum = 0;       /* use signed 64‑bit for safe intermediate */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t q1 = n / d;          /* n/d for divisor d */
            int mu1 = mu[q1];
            if (mu1 != 0)
                sum += (int64_t)mu1 * (int64_t)A252[d];

            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t q2 = n / d2;     /* n/d2 == d */
                int mu2 = mu[q2];
                if (mu2 != 0)
                    sum += (int64_t)mu2 * (int64_t)A252[d2];
            }
        }
    }

    return (seq_t)sum;
}
