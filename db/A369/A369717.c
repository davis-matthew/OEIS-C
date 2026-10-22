/*
 * A369717  The sum of divisors of the smallest powerful number that is a
 *          multiple of n.
 * https://oeis.org/A369717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e||n} * if e == 1: p^2 + p + 1 * else:
 *            (p^(e+1) - 1) / (p - 1) * (multiplicative, derived from a(n) =
 *            sigma(A197863(n)) )
 * Range:     u8 n <= 13, u16 n <= 169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined by OEIS, but safe */

    seq_t result = (seq_t)1;
    idx_t remainder = n;

    /* trial division up to sqrt(remainder) */
    idx_t p = 2;
    while (p <= remainder / p) {
        if (remainder % p == 0) {
            int exp = 0;
            while (remainder % p == 0) {
                remainder /= p;
                ++exp;
            }

            seq_t term;
            if (exp == 1) {
                seq_t pp = (seq_t)p;
                term = pp * pp + pp + (seq_t)1;
            } else {
                /* (p^(exp+1) - 1) / (p - 1)   */
                seq_t numerator = big_pow((seq_t)p, (seq_t)(exp + 1)) - (seq_t)1;
                seq_t denominator = (seq_t)p - (seq_t)1;
                term = numerator / denominator;
            }
            result *= term;
        }

        /* skip even numbers after 2 */
        if (p == 2)
            p = 3;
        else
            p += 2;
    }

    /* if remainder > 1 it is a prime factor > sqrt(n) with exponent 1 */
    if (remainder > 1) {
        seq_t pp = (seq_t)remainder;
        result *= (pp * pp + pp + (seq_t)1);
    }

    return result;
}
