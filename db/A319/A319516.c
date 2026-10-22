/*
 * A319516  Number of integers x such that 1 <= x <= n and gcd(x,n) =
 *          gcd(x+2,n) = gcd(x+6,n) = gcd(x+8,n) = 1.
 * https://oeis.org/A319516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e || n} f(p,e) where f(p,e) = p^(e-1) if p <= 5
 *            (p-4)*p^(e-1) if p > 5 (multiplicative definition)
 * Range:     u8 n <= 262, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1 ; n < 1 is undefined but return 1 */
        return (seq_t)1;

    seq_t result = 1;
    idx_t m = n;

    /* factor 2 */
    if ((m & 1) == 0) {
        unsigned int cnt = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            cnt++;
        }
        /* a(2^cnt) = 2^(cnt-1) */
        seq_t power = 1;
        for (unsigned int i = 0; i < cnt - 1; ++i)
            power *= (seq_t)2;
        result *= power;
    }

    /* odd factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            unsigned int cnt = 0;
            while (m % p == 0) {
                m /= p;
                cnt++;
            }
            /* compute p^(cnt-1) */
            seq_t power = 1;
            for (unsigned int i = 0; i < cnt - 1; ++i)
                power *= (seq_t)p;

            seq_t mult;
            if (p <= 5)
                mult = power;                     /* p^(cnt-1) */
            else
                mult = (seq_t)(p - 4) * power;    /* (p-4)*p^(cnt-1) */
            result *= mult;
        }
    }

    /* remaining prime factor (if any) */
    if (m > 1) {
        /* m is prime, exponent = 1 */
        if (m <= 5)
            /* factor = 1 (p^(1-1)) */
            ;
        else
            result *= (seq_t)(m - 4);
    }

    return result;
}
