/*
 * A379498  a(n) = A276086(1+n) - A276086(A001065(n)), where A276086 is the
 *          primorial base exp-function, and A001065 is the sum of proper
 *          divisors of n.
 * https://oeis.org/A379498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A276086(1+n) - A276086(A001065(n))
 * Range:     i8 n <= 11, i16 n <= 56, i32 n <= 339, i64 n <= 2059,
 *            i128 n <= 9613, big any n
 * Helpers:   build_primes_upto, factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (A001065(n) = sum of proper divisors of n, also = sigma(n) - n) */
/* The implementation uses the primorial‑base expunction function */
/* with a cached list of primes built once. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_pow.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Compute A276086(m) = product of p_i ^ (digit_i) for primorial
   base representation of m (digit_i < p_i).  m must fit in idx_t. */
static seq_t primorial_exp(idx_t m, const uint64_t *primes, size_t prime_cnt)
{
    seq_t result = (seq_t)1;
    idx_t remainder = m;
    for (size_t i = 0; i < prime_cnt && remainder > 0; ++i)
    {
        idx_t prime = primes[i];
        idx_t digit  = remainder % prime;
        remainder  /= prime;
        if (digit)
            result = result * seq_pow((seq_t)prime, (idx_t)digit);
    }
    return result;
}

seq_t a(idx_t n)
{
    /* One‑time prime table initialised only once. */
    static bool primes_inited = false;
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    if (!primes_inited)
    {
        /* 200 000 exceeds the necessary primorial depth for the
           first 10 000 terms comfortably. */
        build_primes_upto((idx_t)200000, &prime_list, &prime_cnt);
        primes_inited = true;
    }

    /* Part 1: A276086(1+n) */
    seq_t part1 = primorial_exp((idx_t)(n + 1), prime_list, prime_cnt);

    /* Part 2: A276086(A001065(n))  where A001065(n) = sigma(n) – n   */
    /* Compute sigma(n) from its prime factorisation. */
    uint64_t pf[64], pe[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, pf, pe, &pf_cnt);

    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < pf_cnt; ++i)
    {
        seq_t p   = (seq_t)pf[i];
        idx_t  e  = (idx_t)(pe[i] + 1);            /* e+1 for the term */
        seq_t p_e = seq_pow(p, e);                 /* p^(e+1) */
        seq_t numer = p_e - (seq_t)1;
        seq_t denom = p - (seq_t)1;
        sigma = sigma * (numer / denom);
    }

    seq_t proper_sum = sigma - (seq_t)n;           /* sigma(n) – n */

    seq_t part2 = primorial_exp((idx_t)proper_sum, prime_list, prime_cnt);

    return part1 - part2;
}
