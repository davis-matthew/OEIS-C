/*
 * A349341  Dirichlet inverse of A026741, which is defined as n if n is odd,
 *          n/2 if n is even.
 * https://oeis.org/A349341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is the Dirichlet inverse of A026741. It is multiplicative
 *            with: a(1) = 1 a(2^e) = -1 (for any e ≥ 1) a(p) = -p (p an odd
 *            prime) a(p^e) = 0 (p odd prime, e > 1) The implementation
 *            factorises n and multiplies these local values
 * Range:     i8 n <= 128, i16 n <= 9377, i32 n <= 9377, i64 n <= 9377,
 *            i128 n <= 9377, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the required term of OEIS A349341 */
seq_t a(idx_t n)
{
    /* Offset 1: 1 → first term */
    if (n == 1)
        return (seq_t)1;

    idx_t t = n;
    seq_t result = (seq_t)1;
    int zero = 0; /* flag that the product will be 0 */

    /* Handle powers of 2 */
    if ((t & 1ULL) == 0) {            /* t is even */
        result *= (seq_t)(-1);        /* a(2^e) = -1 for any e ≥ 1 */
        while ((t & 1ULL) == 0)       /* remove all factors of 2 */
            t >>= 1;
    }

    /* Factorise remaining odd part */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            int exp = 0;
            while (t % p == 0) {
                t /= p;
                exp++;
            }
            if (exp == 1) {
                result *= (seq_t)(- (seq_t)p);   /* a(p) = -p   */
            } else {                            /* p^e, e > 1  */
                result = 0;
                zero = 1;
                break;
            }
        }
    }

    if (!zero && t > 1) {               /* t is a remaining prime > sqrt(original) */
        result *= (seq_t)(- (seq_t)t);   /* a(t) = -t   */
    }

    return result;
}
