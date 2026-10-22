/*
 * A360996  Multiplicative with a(p^e) = 5*e, p prime and e > 0.
 * https://oeis.org/A360996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} 5·e (with a(1)=1)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is a multiplicative function whose value on a prime power p^e is 5·e. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;       /* a(1)=1, a(0) is unused */

    seq_t result = (seq_t)1;
    uint64_t num  = (uint64_t)n;
    uint64_t p    = 2;

    /* trial division up to sqrt(num) */
    while (p * p <= num) {
        if (num % p == 0) {
            uint64_t exp = 0;
            while (num % p == 0) {
                num /= p;
                ++exp;
            }
            result *= (seq_t)(5 * exp);
        }
        ++p;
    }

    if (num > 1)               /* remaining prime factor with exponent 1 */
        result *= (seq_t)(5 * (uint64_t)1);

    return result;
}
