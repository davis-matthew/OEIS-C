/*
 * A399672  Smallest number k of the form lpf(k) * q(k)^n * rad(k) such that
 *          A008479(k) is minimized, where q(k) is the second smallest
 *          distinct prime factor of k and rad = A007947.
 * https://oeis.org/A399672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = prime(i)^2 * prime(i+1)^(n+2) where i is the smallest
 * index such that log(prime(i+1))/log(prime(i)) < (n+2)/(n+1).
 * This satisfies A008479(a(n)) = binomial(n+2,2+n) = (n+2)*(n+5)/2.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* static data for primes and log ratios */
static bool   _primes_init   = false;
static uint64_t *_primes = NULL;
static size_t   _prime_cnt = 0;
static double *_logratio = NULL;

/* build a list of primes up to a fixed limit and pre‑compute log ratios */
static void _init_primes(void)
{
    if (_primes_init)
        return;

    /* limit chosen to comfortably cover n up to about 10 000 */
    const int LIMIT = 300000;            /* less than a megabyte of memory */

    bool *sieve = (bool *)calloc(LIMIT + 1, sizeof(bool));
    for (int i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (int j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;

    /* count primes */
    size_t cnt = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            ++cnt;

    /* store primes */
    _primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t pos = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            _primes[pos++] = (uint64_t)i;
    _prime_cnt = cnt;

    /* pre‑compute log ratios */
    _logratio = (double *)malloc((cnt - 1) * sizeof(double));
    for (size_t k = 0; k + 1 < cnt; ++k)
        _logratio[k] = log((double)_primes[k + 1]) / log((double)_primes[k]);

    free(sieve);
    _primes_init = true;
}

/* sequence function */
seq_t a(idx_t n)
{
    _init_primes();

    /* threshold (n+2)/(n+1) as a double */
    double thresh = (double)(n + 2) / (double)(n + 1);

    /* find the smallest i with logratio[i] < thresh */
    size_t i = 0;
    while (i + 1 < _prime_cnt && _logratio[i] >= thresh)
        ++i;

    /* compute a(n) = prime[i]^2 * prime[i+1]^(n+2) */
    seq_t p_i   = (seq_t)_primes[i];
    seq_t p_ip1 = (seq_t)_primes[i + 1];

    seq_t pow_part = seq_pow(p_ip1, n + 2);
    seq_t square   = seq_mul(p_i, p_i);
    return seq_mul(square, pow_part);
}
