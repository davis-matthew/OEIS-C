/*
 * A390614  a(n) = Kronecker symbol (-35/n) = (n/35).
 * https://oeis.org/A390614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = Kronecker symbol (-35/n) = (n/35)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   legendre_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/legendre_small.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 by convention */
    if (n == 0)
        return (seq_t)0;

    /* If n shares a factor with 35, the Kronecker symbol is 0. */
    if ((n % 5ULL) == 0 || (n % 7ULL) == 0)
        return (seq_t)0;

    /* (n/35) = (n/5) * (n/7) for all n coprime with 35 */
    int res = 1;
    res *= legendre_small(n, 5ULL);
    res *= legendre_small(n, 7ULL);

    return (seq_t)res;
}
