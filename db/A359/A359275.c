/*
 * A359275  a(n) = greatest integer k such that (n+k)^2 <= n^3.
 * https://oeis.org/A359275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -n + floor(n^(3/2)) (OEIS A359275)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t tn = n;
    /* Compute n^3 exactly in seq_t; the values used in the tests
       fit without overflow for the supported seq_t sizes. */
    seq_t n3 = tn * tn * tn;
    seq_t root = floor_sqrt(n3);
    return root - tn;
}
