/*
 * A273224  a(n) = p*(p - 1)*(4051*p^4 - 4130*p^3 + 1445*p^2 - 190*p +
 *          264)/720, where p = prime(n).
 * https://oeis.org/A273224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*(p - 1)*(4051*p^4 - 4130*p^3 + 1445*p^2 - 190*p +
 *            264)/720
 * Range:     u8 none, u16 n <= 1, u32 n <= 10, u64 n <= 199, u128 n <= 10000,
 *            big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t p  = (seq_t)get_prime(n);
    seq_t p1 = p - 1;

    /* compute polynomial 4051*p^4 - 4130*p^3 + 1445*p^2 - 190*p + 264 */
    seq_t p2 = p * p;          /* p^2 */
    seq_t p3 = p2 * p;         /* p^3 */
    seq_t p4 = p3 * p;         /* p^4 */

    seq_t poly = 4051 * p4;
    poly -= 4130 * p3;
    poly += 1445 * p2;
    poly -= 190 * p;
    poly += 264;

    seq_t M   = 720;   /* remaining factor of the denominator */
    seq_t res = 1;

    /* multiply a factor and cancel as many common divisors with the
       remaining denominator as possible. */
    #define MUL_CANCEL(factor)                        \
        {                                               \
            seq_t g = gcd((factor), M);                 \
            (factor) /= g;                              \
            M /= g;                                     \
            res *= (factor);                            \
            g = gcd(res, M);                            \
            res /= g;                                   \
            M /= g;                                     \
        }

    MUL_CANCEL(p);
    MUL_CANCEL(p1);
    MUL_CANCEL(poly);

    #undef MUL_CANCEL

    if (M != 1) {
        res /= M;
    }

    return res;
}
