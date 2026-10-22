/*
 * A309732  Expansion of Sum_{k>=1} k^2 * x^k/(1 - x^k)^3.
 * https://oeis.org/A309732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ( n * d(n) + σ(n) ) / 2 where d(n) = number of
 *            divisors of n, σ(n) = sum of divisors of n
 * Range:     u8 n <= 9, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes the OEIS sequence A309732 for index n (n >= 1). */
seq_t a(idx_t n)
{
    /* Convert index to seq_t once for repeated use. */
    seq_t sn = (seq_t)n;

    idx_t temp = n;            /* temporary copy for factorisation   */
    seq_t sigma = 1;           /* product of divisor sums            */
    seq_t d = 1;               /* product of (exponent+1)            */

    /* Trial division: factorise n into primes. */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            seq_t sum = 1;        /* p^0 */
            seq_t power = 1;      /* current p^k */

            while (temp % p == 0) {
                temp /= p;
                exp++;
                power *= (seq_t)p;  /* power = p^k   */
                sum += power;      /* add next term p^k */
            }

            d *= (exp + 1);
            sigma *= sum;
        }
    }

    /* If remaining temp > 1, it is a prime factor. */
    if (temp > 1) {
        d *= 2;                       /* exponent 1 -> (1+1) */
        sigma *= (1 + (seq_t)temp);    /* sum of 1 and temp */
    }

    /* a(n) = n * ( n * d + σ ) / 2   */
    seq_t inner = sn * d + sigma;      /* n*d + σ  */
    seq_t result = sn * inner / 2;     /* *n / 2   */

    return result;
}
