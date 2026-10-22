/*
 * A354090  a(n) = A007947(n)^(A051904(n) - 1), where A007947 is squarefree
 *          kernel and A051904 is minimum prime exponent.
 * https://oeis.org/A354090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947(n)^(A051904(n)-1) aka a(n) = n / A304776(n) (for
 *            n >= 1, a(1) = 1)
 * Range:     u8 n <= 511, u16 n <= 9518, u32 n <= 9518, u64 n <= 9518,
 *            u128 n <= 9518, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1 by definition */
        return (seq_t)1;

    idx_t tmp = n;
    idx_t rad = 1;                 /* squarefree kernel */
    idx_t min_exp = (idx_t)-1;     /* infinity */

    /* trial division up to sqrt(tmp) */
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            rad *= p;
            if (exp < min_exp)
                min_exp = exp;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        rad *= tmp;                /* new prime factor */
        if (1 < min_exp)
            min_exp = 1;
    }

    /* exponent for the power; exponent may be 0 */
    idx_t exp = min_exp - 1;
    if (exp == 0)
        return (seq_t)1;           /* a(n) = 1 */

    /* power rad^exp using exponentiation by squaring */
    seq_t base = (seq_t)rad;
    seq_t result = (seq_t)1;
    idx_t e = exp;
    while (e) {
        if (e & 1)
            result *= base;
        e >>= 1;
        if (e)
            base *= base;
    }
    return result;
}
