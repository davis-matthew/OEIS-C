/*
 * A372441  Number of binary indices (binary weight) of n minus number of
 *          prime indices (bigomega) of n.
 * https://oeis.org/A372441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A000120(n) – A001222(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* bigomega gives the number of prime factors (with multiplicity) of n */
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Count bits set to 1 in n (binary weight) */
    seq_t pop = 0;
    idx_t x = n;
    while (x != 0) {
        pop += 1;
        x &= x - 1;          /* remove the lowest set bit */
    }

    seq_t bo = bigomega(n);    /* prime factor multiplicity count */
    return pop - bo;
}
