/*
 * A326143  a(n) = A326142(n) - n, where A326142 gives the sum of all other
 *          divisors of n except its largest squarefree divisor.
 * https://oeis.org/A326143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = sigma(n) - rad(n) - n where sigma(n) is the sum of all
 *            divisors of n and rad(n) is the largest squarefree divisor of n
 * Range:     i8 n <= 48, i16 n <= 8730, i32 n <= 8730, i64 n <= 8730,
 *            i128 n <= 8730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division using odd numbers, avoiding overflow in p*p by
   comparing p <= temp/p. All arithmetic is performed in seq_t, compatible with signed or
   arbitrary‑precision types. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* undefined, but harmless */
        return (seq_t)0;

    seq_t sigma = 1;          /* product of sigma factors */
    seq_t rad   = 1;          /* product of distinct prime factors */
    idx_t temp  = n;

    /* factor 2 first */
    if ((temp & (idx_t)1) == 0) {
        int e = 0;
        while ((temp & (idx_t)1) == 0) {   /* temp % 2 == 0 */
            temp >>= 1;                    /* temp /= 2 */
            e++;
        }
        seq_t p = 2;
        seq_t ppow = 1;
        for (int i = 0; i < e + 1; i++)
            ppow *= p;
        seq_t denom = p - 1;                      /* p - 1 is positive */
        seq_t factor = (ppow - 1) / denom;         /* (p^(e+1)-1)/(p-1) */
        sigma *= factor;
        rad   *= p;
    }

    /* factor odd primes */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            seq_t pseq = (seq_t)p;
            seq_t ppow = 1;
            for (int i = 0; i < e + 1; i++)
                ppow *= pseq;
            seq_t denom = pseq - 1;
            seq_t factor = (ppow - 1) / denom;
            sigma *= factor;
            rad   *= pseq;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        seq_t pseq = (seq_t)temp;
        sigma *= (pseq + 1);  /* (p^2-1)/(p-1) = p+1 */
        rad   *= pseq;
    }

    seq_t result = sigma - rad - (seq_t)n;
    return result;
}
