/*
 * A289451  a(n) is the number of permutations of length n that avoid the
 *          pattern 231 and the mesh pattern (12, 409) or the same sequence
 *          for the mesh pattern (12, 473).
 * https://oeis.org/A289451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n, n)/(n+1) - (n-1)*n/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t cat = catalan(n);
    seq_t sub = ((seq_t)n * (seq_t)(n - 1)) / 2;
    return cat - sub;
}
