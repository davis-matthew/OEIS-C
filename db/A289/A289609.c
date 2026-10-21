/*
 * A289609  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 285) or the same sequence
 *          for the mesh patterns (12, 339), (12, 369), (12, 405).
 * https://oeis.org/A289609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Catalan(n) - 2^(n-2) - 2*n + 5 (for n > 2) a(0) = a(1) =
 *            a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;

    seq_t cat   = catalan(n);                     /* Catalan number C(n) */
    seq_t two   = pow2(n - 2);                    /* 2^(n-2)          */
    seq_t sub   = (seq_t)((2 * n) - 5);           /* 2*n - 5          */

    return cat - two - sub;
}
