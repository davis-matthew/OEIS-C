/*
 * A347291  Multiplicative function defined by a(p) = 2 and a(p^k) = p^(k-1)
 *          for k >= 2.
 * https://oeis.org/A347291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers in n: a(p^k) = 2 if k == 1
 *            a(p^k) = p^(k-1) if k >= 2 (multiplicative over distinct primes)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */
#include "../utils/pow_seq_fast.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;     /* work on a native integer for factorisation */
    seq_t    result = 1;

    /* handle factor 2 separately */
    uint64_t exp = 0;
    while ((m & 1) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp > 0) {
        if (exp == 1)
            result *= (seq_t)2;
        else
            result *= pow_seq_fast((seq_t)2, exp - 1);
    }

    /* iterate over odd potential divisors */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            exp = 0;
            while (m % d == 0) {
                m /= d;
                exp++;
            }
            if (exp == 1)
                result *= (seq_t)2;
            else
                result *= pow_seq_fast((seq_t)d, exp - 1);
        }
    }

    /* if anything remains, it is a prime factor with exponent 1 */
    if (m > 1)
        result *= (seq_t)2;

    return result;
}
