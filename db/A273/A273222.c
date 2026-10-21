/*
 * A273222  a(n) = p*(p - 1)*(73*p^2 - 45*p + 14)/24, where p = prime(n).
 * https://oeis.org/A273222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*(p - 1)*(73*p^2 - 45*p + 14)/24 , where p = prime(n)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 44, u64 n <= 5096,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; n==0 is undefined */
        return 0;

    /* prime(n) fits in 32 bits for the ranges required */
    seq_t p   = (seq_t)get_prime(n);
    seq_t term = 73 * p * p - 45 * p + 14;

    /* efficiently cancel the denominator 24 from the three factors */
    seq_t factors[3] = { p, p - 1, term };
    seq_t denom = 24;
    seq_t result = 1;

    for (int i = 0; i < 3; ++i) {
        seq_t g = gcd(factors[i], denom);
        if (g > 1) {
            factors[i] /= g;
            denom /= g;
        }
        result *= factors[i];
    }

    /* for values that fit in seq_t the denominator will be 1 here */
    if (denom > 1)
        result /= denom;

    return result;
}
