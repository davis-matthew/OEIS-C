/*
 * A329152  a(n) = Sum_{i=1..n-1} Sum_{j=1..i-1} [1 == i*j (mod n)], where []
 *          is the Iverson bracket.
 * https://oeis.org/A329152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (phi(n) – C(n))/2, where phi(n) is Euler's totient and
 *            C(n) is the number of solutions to x² ≡ 1 (mod n)
 * Range:     u8 n <= 481, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n <= 1)
        return 0;   /* a(1) = 0, and a(0) is never queried */

    /* phi(n) – start with n and subtract n/p for each distinct prime p */
    seq_t phi = (seq_t)n;

    /* C(n) – count solutions of x² ≡ 1 (mod n) */
    seq_t self_inv = 1;         /* start with the multiplicative identity */

    /* Work on a copy of n for factorisation */
    idx_t temp = n;

    /* Handle the prime 2 separately to compute its exponent */
    idx_t exp2 = 0;
    while ((temp & 1) == 0) {
        exp2++;
        temp >>= 1;
    }

    /* Update phi for each distinct prime factor */
    if (exp2 > 0)
        phi -= phi / (seq_t)2;          /* removing factor 2 once */

    /* Update self_inv according to the exponent of 2 */
    if (exp2 == 0 || exp2 == 1) {
        /* factor remains 1, no change */
    } else if (exp2 == 2) {
        self_inv <<= 1;                 /* multiply by 2 */
    } else { /* exp2 >= 3 */
        self_inv <<= 2;                 /* multiply by 4 */
    }

    /* Factor remaining odd part */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            /* p is a distinct prime factor */
            while (temp % p == 0) {
                temp /= p;
            }
            /* Update phi once per distinct odd prime */
            phi -= phi / (seq_t)p;

            /* Each odd prime contributes a factor of 2 to self_inv */
            self_inv <<= 1;
        }
    }

    /* If a prime factor > sqrt(original n) remains */
    if (temp > 1) {
        /* temp is a prime factor */
        phi -= phi / (seq_t)temp;
        self_inv <<= 1;
    }

    /* Final result: (phi - self_inv) / 2 */
    return (phi - self_inv) / 2;
}
