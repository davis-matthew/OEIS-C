/*
 * A388977  a(n) = n * A033885(A003961(n)), where A033885(n) = 3*n-sigma(n),
 *          and A003961 is fully multiplicative with a(p) = nextprime(p).
 * https://oeis.org/A388977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n * (3 * A003961(n) - sigma(A003961(n)))
 * Range:     i8 n <= 6, i16 n <= 63, i32 n <= 4095, i64 n <= 9132,
 *            i128 n <= 9132, big any n
 * Helpers:   build_primes_upto, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961(n) is multiplicative with A003961(p) = nextprime(p). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit,&primes,&cnt) */
#include "../utils/next_prime.h"          /* next_prime(p) -> next prime > p */

/* Static prime table for factorisation (up to 200 000). */
static uint64_t *primes = NULL;
static size_t prime_count = 0;
static void init_primes(void)
{
    if (primes != NULL)
        return;                      /* already initialised */

    /* 200 000 is ample for factorising n up to about 10 000 with trial division. */
    build_primes_upto(200000, &primes, &prime_count);
}

/* Integer power for seq_t using repeated multiplication (exponent is small). */
static seq_t pow_seq_uint(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/* Compute a(n) for the OEIS sequence A388977. */
seq_t a(idx_t n)
{
    init_primes();          /* make sure the prime table is ready */

    if (n == 0)
        return 0;           /* undefined for n = 0, but harmless */

    uint64_t rem = (uint64_t)n;   /* use uint64_t for trial division */

    seq_t m = 1;               /* A003961(n) */
    seq_t sigma_m = 1;         /* sigma(A003961(n)) */

    /* Trial‑division by the precomputed primes. */
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= rem; ++i)
    {
        uint64_t p = primes[i];
        if (rem % p != 0)
            continue;

        /* Count the exponent of p in n. */
        int exp = 0;
        while (rem % p == 0)
        {
            rem /= p;
            ++exp;
        }

        /* Calculate the contribution of this prime to A003961(n). */
        uint64_t nextp = next_prime(p);           /* nextprime(p) */
        seq_t pow_m = pow_seq_uint((seq_t)nextp, exp); /* (nextp)^exp */

        m *= pow_m;                              /* update A003961(n) */

        /* Update sigma(A003961(n)) using the closed form for σ(nextp^exp). */
        seq_t pow_nextp_exp_plus1 = pow_seq_uint((seq_t)nextp, exp + 1);
        seq_t term_sigma = (pow_nextp_exp_plus1 - 1) / ((seq_t)nextp - 1);
        sigma_m *= term_sigma;
    }

    /* If a prime factor remains greater than sqrt(original n). */
    if (rem > 1)
    {
        uint64_t p = rem;           /* remaining prime factor */
        uint64_t nextp = next_prime(p);

        seq_t pow_m = nextp;        /* exponent 1 */
        m *= pow_m;

        /* σ(nextp^1) = nextp + 1 */
        seq_t term_sigma = nextp + 1;
        sigma_m *= term_sigma;
    }

    /* A033885(m) = 3*m - σ(m) */
    seq_t temp = (seq_t)3 * m - sigma_m;

    return (seq_t)n * temp;
}
