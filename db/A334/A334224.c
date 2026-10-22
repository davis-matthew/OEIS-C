/*
 * A334224  Consider a graph as defined in A306302 formed from a row of n
 *          adjacent congruent squares with the diagonals of all possible
 *          rectangles; a(n) is the minimum edge length of the squares such
 *          that the vertices formed by all intersections have integer x and y
 *          coordinates.
 * https://oeis.org/A334224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003418(2n-1) = LCM(1,2,…,2n-1) for n>1; a(1)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   lcm_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/lcm_upto.h"

/* Compute LCM of all integers from 1 to m (inclusive) */

/* Sequence function required by the test harness */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)2;

    idx_t m = (idx_t)2 * n - (idx_t)1;
    return lcm_upto(m);
}
