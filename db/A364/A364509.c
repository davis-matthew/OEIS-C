/*
 * A364509  Square array read by ascending antidiagonals: T(n,k) = (2*k)!/k!^2
 *          * ( (2*n*k)! * ((n + 2)*k)! )/( (n*k)! * ((n + 1)*k)!^2 ) for n, k
 *          > = 0.
 * https://oeis.org/A364509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(d - i, i) where d = floor((sqrt(8*n+1)-1)/2) i = n -
 *            d*(d+1)/2 T(n,k) = ((2*k)!/k!^2) * ((2*n*k)! *
 *            ((n+2)*k)!)/((n*k)! * ((n+1)*k)!^2) The array is read by
 *            ascending antidiagonals with k increasing from 0 upwards and n
 *            decreasing from d downwards
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 38, u64 n <= 70, u128 n <= 142,
 *            big any n
 * Helpers:   _floor_isqrt_u64, add_fact_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/add_fact_exp.h"

#define PRIME_LIMIT 50000
#define MAX_PRIMES 6000

static int once_primes[MAX_PRIMES];
static int once_cnt = 0;

/* Initialize prime table once */
static void init_primes(void)
{
    if (once_cnt > 0) return;
    bool is_prime[PRIME_LIMIT + 1];
    memset(is_prime, true, sizeof(is_prime));
    if (PRIME_LIMIT >= 0) is_prime[0] = false;
    if (PRIME_LIMIT >= 1) is_prime[1] = false;
    for (int p = 2; (long long)p * p <= PRIME_LIMIT; ++p)
    {
        if (is_prime[p])
        {
            for (int m = p * p; m <= PRIME_LIMIT; m += p)
                is_prime[m] = false;
        }
    }
    for (int p = 2; p <= PRIME_LIMIT; ++p)
    {
        if (is_prime[p])
        {
            once_primes[once_cnt++] = p;
        }
    }
}

/* Multiply prime power by fast exponentiation */
static seq_t pow_seq_small(seq_t base, unsigned int exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t idx)
{
    /* Locate diagonal d */
    idx_t s = _floor_isqrt_u64(8 * idx + 1);
    idx_t d = (s - 1) >> 1;           /* (s-1)/2 */
    idx_t prev = d * (d + 1) / 2;
    idx_t i = idx - prev;             /* offset inside diagonal */
    idx_t k = i;
    idx_t n = d - i;

    init_primes();

    int exps[MAX_PRIMES];
    memset(exps, 0, once_cnt * sizeof(int));

    add_fact_exp(2 * k,       +1, once_primes, once_cnt, exps);    /* (2k)! */
    add_fact_exp(k,           -2, once_primes, once_cnt, exps);    /* / (k!)^2 */
    add_fact_exp(2 * n * k,   +1, once_primes, once_cnt, exps);    /* * (2nk)! */
    add_fact_exp((n + 2) * k, +1, once_primes, once_cnt, exps);    /* * ((n+2)k)! */
    add_fact_exp(n * k,       -1, once_primes, once_cnt, exps);    /* / (nk)!   */
    add_fact_exp((n + 1) * k, -2, once_primes, once_cnt, exps);    /* / ((n+1)k)!^2 */

    seq_t result = 1;
    for (int pi = 0; pi < once_cnt; ++pi)
    {
        int e = exps[pi];
        if (e <= 0) continue;
        int p = once_primes[pi];
        result *= pow_seq_small((seq_t)p, (unsigned int)e);
    }
    return result;
}
