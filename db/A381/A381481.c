/*
 * A381481  Dirichlet g.f.: zeta(s) * Product_{p prime} (1 - 1/p^(2*s) -
 *          1/p^(3*s)).
 * https://oeis.org/A381481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} dir(p^e) where dir(p^e) = 1 if e=1, 0 if e=2,
 *            -1 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 (product over empty set is 1) */
    if (n == 1)
        return (seq_t)1;

    seq_t sign = 1;
    /* Use a mutable copy of n for factorisation */
    idx_t rem = n;

    /* Trial division. We loop p from 2 up to sqrt(rem).
       To avoid overflow of p*p for large rem, use 128‑bit arithmetic. */
    for (idx_t p = 2;
         (unsigned __int128)p * p <= rem;
         ++p)
    {
        if (rem % p != 0)
            continue;

        /* Count exponent of prime p */
        idx_t e = 0;
        while (rem % p == 0) {
            rem /= p;
            ++e;
        }

        /* If any exponent equals 2, result is zero */
        if (e == 2)
            return (seq_t)0;

        /* For exponent ≥3, multiply sign by -1 */
        if (e >= 3)
            sign = -sign;
    }

    /* If after the loop rem > 1, it is a prime factor with exponent 1.
       It contributes nothing to the sign. */

    return sign;
}
