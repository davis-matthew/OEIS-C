/*
 * A300828  Multiplicative with a(p^2) = 1, a(p^3) = -2 and a(p^e) = 0 when e
 *          = 1 or e > 3.
 * https://oeis.org/A300828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} f(e) where f(2)=1, f(3)=−2, f(1)=0, f(e>3)=0.
 *            Therefore a(n)=0 unless every prime exponent is 2 or 3; in that
 *            case a(n)=(-2)^{#primes with exponent 3}
 * Range:     i8 n <= 9922, i16 n <= 9922, i32 n <= 9922, i64 n <= 9922,
 *            i128 n <= 9922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t num = (uint64_t)n;

    /* factor 2 */
    int exp = 0;
    while ((num & 1ULL) == 0) {
        exp++;
        num >>= 1;
    }
    if (exp) {
        if (exp == 2)
            /* multiply by 1 – no change */;
        else if (exp == 3)
            result *= (seq_t)-2;
        else
            return (seq_t)0;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= num / p; p += 2) {
        if (num % p == 0) {
            exp = 0;
            while (num % p == 0) {
                exp++;
                num /= p;
            }
            if (exp == 2)
                /* multiply by 1 */;
            else if (exp == 3)
                result *= (seq_t)-2;
            else
                return (seq_t)0;
        }
    }

    /* if remaining factor > 1, it is a prime with exponent 1 */
    if (num > 1)
        return (seq_t)0;

    return result;
}
