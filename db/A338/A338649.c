/*
 * A338649  Number of divisors of n which are greater than 5.
 * https://oeis.org/A338649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are greater than 5
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* out of offset guard */

    idx_t m = n;                              /* temporary copy for factorization */
    seq_t d = 1;                              /* total divisor count */

    /* factor 2 */
    idx_t e = 0;
    while ((m & 1) == 0) { e++; m >>= 1; }
    if (e) d *= (e + 1);

    /* factor 3 */
    e = 0;
    while (m % 3 == 0) { e++; m /= 3; }
    if (e) d *= (e + 1);

    /* factor 5 */
    e = 0;
    while (m % 5 == 0) { e++; m /= 5; }
    if (e) d *= (e + 1);

    /* remaining odd primes */
    for (idx_t p = 7; p * p <= m; p += 2) {
        e = 0;
        while (m % p == 0) { e++; m /= p; }
        if (e) d *= (e + 1);
    }
    if (m > 1) d *= 2;                       /* last prime factor */

    /* count divisors <= 5 that divide n */
    seq_t small = 1;                          /* divisor 1 always divides n */
    if ((n & 1) == 0) small++;                /* 2 divides n */
    if (n % 3 == 0) small++;                  /* 3 divides n */
    if (n % 4 == 0) small++;                  /* 4 divides n */
    if (n % 5 == 0) small++;                  /* 5 divides n */

    return d - small;                        /* divisors > 5 */
}
