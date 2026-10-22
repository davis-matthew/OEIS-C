/*
 * A363618  Expansion of Sum_{k>0} x^(4*k)/(1+x^k)^5.
 * https://oeis.org/A363618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^d * binomial(d,4)
 * Range:     i8 n <= 5, i16 n <= 15, i32 n <= 470, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 0 for n < 4, otherwise sum over all divisors d of n.   */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute binomial(d,4) for d >= 4; returns 0 otherwise. */
static seq_t binom4(seq_t d)
{
    if (d < 4) return 0;
    seq_t r = d * (d - 1) * (d - 2) * (d - 3);
    r /= 24;          /* division is exact (4! = 24) */
    return r;
}

/* Recursively enumerate divisors of n.
   primes   : array of distinct prime factors of n
   exps     : corresponding exponents
   idx      : current position in the arrays
   total    : total number of distinct prime factors
   cur      : current divisor being constructed
   sum_ptr  : pointer to the running total */
static void enumerate_divisors(const uint64_t *primes,
                               const uint64_t *exps,
                               size_t idx,
                               size_t total,
                               seq_t cur,
                               seq_t *sum_ptr)
{
    if (idx == total) {
        seq_t d = cur;
        seq_t term = binom4(d);
        if (term != 0) {
            if (d & 1)          /* odd divisor -> negative sign */
                *sum_ptr -= term;
            else
                *sum_ptr += term;
        }
        return;
    }

    seq_t p = (seq_t)primes[idx];
    seq_t mult = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(primes, exps, idx + 1, total, cur * mult, sum_ptr);
        mult *= p;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n < 4 the result is 0 because every divisor d < 4
       yields binomial(d,4) = 0. */
    if (n < 4) return 0;

    /* Factor n.  The maximum number of distinct prime factors of a
       64‑bit integer is less than 16, so a fixed-size array is safe. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t  cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    enumerate_divisors(primes, exps, 0, cnt, 1, &result);
    return result;
}
