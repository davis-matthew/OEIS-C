/*
 * A394591  Sum of the distinct prime divisors of C(3*n, n).
 * https://oeis.org/A394591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct prime divisors of binomial(3*n, n)
 * Range:     u8 n <= 17, u16 n <= 364, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper provided by the build environment – generates all primes <= limit.
   The header filename follows the convention mentioned in the problem. */
#include "../utils/build_primes_upto.h"

/* Compute a(n) for n >= 1:
 * a(n) = sopf( binomial(3*n, n) ) = sum of distinct prime divisors of C(3n,n)
 */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n==0 we return 0, but this value is never
       requested. */
    if (n == 0)
        return (seq_t)0;

    /* Upper bound for primes needed in the factorisation of C(3n,n). */
    idx_t limit = 3 * n;

    /* Generate all primes up to 'limit'. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(limit, &primes, &prime_cnt);

    seq_t sum = 0;

    /* For each prime compute its exponent in C(3n,n) using Legendre's
       formula: v_p((3n)!) – v_p(n!) – v_p((2n)!).  If that exponent is
       positive, add the prime to the sum. */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];

        idx_t exp_3n = 0, exp_2n = 0, exp_n = 0;
        idx_t t;

        t = limit;            /* 3n */
        while (t)
        {
            t /= p;
            exp_3n += t;
        }

        t = 2 * n;            /* 2n */
        while (t)
        {
            t /= p;
            exp_2n += t;
        }

        t = n;                /* n */
        while (t)
        {
            t /= p;
            exp_n += t;
        }

        idx_t exp = exp_3n - exp_2n - exp_n;
        if (exp > 0)
            sum += (seq_t)p;   /* sum distinct primes only once */
    }

    free(primes);
    return sum;
}
