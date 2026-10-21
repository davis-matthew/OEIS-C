/*
 * A290953  The number of permutations in S_n for which the number of reduced
 *          words is maximized with respect to the numbers of braid and
 *          commutation classes: |R(w)| = |B(w)| * |C(w)|.
 * https://oeis.org/A290953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = catalan(n) + n - 2 for n > 1; a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    return catalan(n) + (seq_t)n - 2;
}
