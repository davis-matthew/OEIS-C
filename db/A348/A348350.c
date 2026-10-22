/*
 * A348350  a(n) = Sum_{d|n} d^(sigma(d) - 1).
 * https://oeis.org/A348350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(sigma(d) - 1) (OEIS A348350)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided utilities `big_pow`, `sigma_of`,
   and `factor_u64`.  It computes the prime factorisation of `n`,
   generates all divisors, evaluates each term and sums them. */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/big_pow.h"
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

/* --------------------------------------------------------------- */
/* Main function computing a(n)                                 */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined below the offset; safe return   */
        return (seq_t)0;

    /* ----------------------------------------------------------- */
    /* Factor n into a list of primes and exponents.              */
    /* ----------------------------------------------------------- */
    /* The maximum number of distinct prime factors for n ≤ 10000
       is < 7, so an array of size 32 is sufficient. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* ----------------------------------------------------------- */
    /* Generate all divisors of n.                                 */
    /* ----------------------------------------------------------- */
    /* The divisor count of n ≤ 10000 is tiny (≤ 128), so a fixed
       array of size 256 is safe. */
    uint32_t divs[256];
    size_t div_cnt = 1;
    divs[0] = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint32_t p = (uint32_t)primes[i];
        uint32_t e = (uint32_t)exps[i];
        size_t old = div_cnt;
        uint32_t pow = 1;

        /* For each exponent k = 1 .. e add p^k * existing divisors */
        for (uint32_t k = 1; k <= e; ++k)
        {
            pow *= p;
            for (size_t j = 0; j < old; ++j)
                divs[div_cnt++] = divs[j] * pow;
        }
    }

    /* ----------------------------------------------------------- */
    /* Sum d^(sigma(d)-1) over all divisors.                       */
    /* ----------------------------------------------------------- */
    seq_t total = (seq_t)0;

    for (size_t i = 0; i < div_cnt; ++i)
    {
        seq_t d    = (seq_t)divs[i];
        seq_t sig = sigma_of((idx_t)divs[i]);   /* sigma(d) as seq_t   */
        seq_t exp = sig - (seq_t)1;             /* sigma(d)-1          */
        seq_t term = big_pow(d, exp);
        total += term;
    }

    return total;
}
