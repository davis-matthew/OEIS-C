/*
 * A327276  a(n) = Sum_{d|n, d odd} mu(d) * mu(n/d).
 * https://oeis.org/A327276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d|n, d odd} mu(d) * mu(n/d)
 * Multiplicative with prime‑power values:
 *   a(2^e) = -1 (e = 1), 0 (e > 1)
 *   a(p^e) = -2 (e = 1), 1 (e = 2), 0 (e > 2) for odd prime p
 */

#include <stdint.h>
#include <stddef.h>   /* for size_t, just in case */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for offset 1; return 0 */
        return 0;

    uint64_t m = n;       /* use native integer for factorization */
    seq_t result = 1;     /* multiplicative identity */

    /* handle factor 2 */
    unsigned int exp2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++exp2;
    }
    if (exp2) {
        seq_t fac = (exp2 == 1) ? -1 : 0;
        result *= fac;
        if (fac == 0)      /* product becomes zero, early exit */
            return 0;
    }

    /* factor remaining odd part */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p != 0)
            continue;
        unsigned int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        seq_t fac;
        if (exp == 1)
            fac = -2;
        else if (exp == 2)
            fac = 1;
        else
            fac = 0;
        result *= fac;
        if (fac == 0)
            return 0;
    }

    /* any remaining prime factor > sqrt(n) */
    if (m > 1) {                 /* prime with exponent 1 */
        result *= (seq_t)-2;
    }

    return result;
}
