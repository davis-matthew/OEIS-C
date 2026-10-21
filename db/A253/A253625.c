/*
 * A253625  Expansion of psi(q^2) * f(-q, q^2)^2 / f(-q, -q^5) in powers of q
 *          where psi(), f() are Ramanujan theta functions.
 * https://oeis.org/A253625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = -b(n) for n>0, where b is multiplicative with
 *            b(2^e) = -3 (e>0) b(3^e) = 1 b(p^e) = e+1 if p ≡ 1 (mod 6)
 *            b(p^e) = (1+(-1)^e)/2 if p ≡ 5 (mod 6) b(p^e) = 1 otherwise Thus
 *            a(n) is computed by factoring n and multiplying the appropriate
 *            local contributions
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    idx_t tmp = n;
    seq_t result = 1;

    /* factor 2 */
    idx_t e = 0;
    while (tmp % 2 == 0) {
        ++e;
        tmp /= 2;
    }
    if (e > 0)
        result *= (seq_t)(-3);

    /* factor 3 */
    e = 0;
    while (tmp % 3 == 0) {
        ++e;
        tmp /= 3;
    }
    /* b(3^e) = 1, no change */

    /* factor remaining odd primes */
    for (idx_t p = 5; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                ++e;
                tmp /= p;
            }
            if (p % 6 == 1) {
                result *= (seq_t)(e + 1);
            } else if (p % 6 == 5) {
                if (e & 1) {          /* (1+(-1)^e)/2 = 0 for odd e */
                    result = 0;
                    break;
                }
                /* even e gives factor 1, no change */
            }
        }
    }

    /* final remaining prime factor, if any */
    if (result != 0 && tmp > 1) {
        idx_t p = tmp;
        if (p == 2)
            result *= (seq_t)(-3);
        else if (p == 3) {
            /* no change */
        } else if (p % 6 == 1) {
            result *= (seq_t)2;      /* e = 1 ⇒ e+1 = 2 */
        } else if (p % 6 == 5) {
            /* e = 1 is odd, hence b = 0 */
            result = 0;
        }
    }

    return -result;
}
