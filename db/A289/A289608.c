/*
 * A289608  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 313) or the same sequence
 *          for the mesh pattern (12, 403).
 * https://oeis.org/A289608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=8 For n>4: a(n) =
 *            Catalan(n) − 2^(n-2) − 2*n + 4 (Catalan(n) is given by the
 *            utility function catalan)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"   /* catalan(idx_t) */
#include "../utils/pow2.h"      /* pow2(idx_t) */

seq_t a(idx_t n)
{
    /* small n handled explicitly */
    if (n < 5) {
        /* value array for n=0..4 */
        static const seq_t small_vals[5] = {1, 1, 1, 1, 8};
        return small_vals[n];
    }

    /* n >= 5 */
    seq_t cat = catalan(n);          /* Catalan(n) */
    seq_t p2  = pow2(n - 2);        /* 2^(n-2) */
    seq_t two_n = (seq_t)2 * (seq_t)n;   /* 2*n */

    /* a(n) = cat - p2 - two_n + 4 */
    seq_t res = cat;
    res -= p2;
    res -= two_n;
    res += (seq_t)4;
    return res;
}
