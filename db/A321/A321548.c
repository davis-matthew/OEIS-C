/*
 * A321548  a(n) = Sum_{d|n} (-1)^(d-1)*d^9.
 * https://oeis.org/A321548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * d^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 127,
 *            i128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the multiplicative property:
 *   a(p^e) = 1 + p^9 + p^18 + … + p^(9e)   for odd primes p
 *   a(2^e) = 1 – (2^9 + 2^18 + … + 2^(9e))  (all terms negative except the leading 1)
 */

#include "../utils/big_pow.h"
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of offset range */

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Handle factor 2 separately */
    if ((temp & 1) == 0) {
        size_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        /* 2^9 = 512 */
        seq_t pow = (seq_t)512;
        seq_t sum = (seq_t)0;
        seq_t current = pow;
        for (size_t i = 0; i < e; ++i) {
            sum += current;
            current *= pow;            /* next power of 512 */
        }
        seq_t a2 = (seq_t)1 - sum;
        result *= a2;
    }

    /* Process odd prime factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            seq_t pow9 = big_pow((seq_t)p, (seq_t)9);   /* p^9 */
            seq_t pow = (seq_t)1;
            seq_t sum = (seq_t)1;                    /* first term */
            for (size_t i = 0; i < e; ++i) {
                pow *= pow9;                         /* next power of p^9 */
                sum += pow;
            }
            result *= sum;
        }
    }

    /* Remaining prime factor if any */
    if (temp > 1) {
        seq_t pow9 = big_pow((seq_t)temp, (seq_t)9);
        seq_t sum = (seq_t)1 + pow9;
        result *= sum;
    }

    return result;
}
