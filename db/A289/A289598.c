/*
 * A289598  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 363) or the same sequence
 *          for the mesh patterns (12, 399), (12, 429), (12, 483).
 * https://oeis.org/A289598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) - C(n-1) - n + 2, a(0) = 1, where C(k) is the
 *            Catalan number A000108. Compute by calling the provided catalyst
 *            routine
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t cat_n = catalan(n);
    seq_t cat_n_minus1 = catalan(n - 1);
    seq_t res = cat_n - cat_n_minus1 - (seq_t)n + 2;
    return res;
}
