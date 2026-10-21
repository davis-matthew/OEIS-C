/*
 * A289607  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 279) or the same sequence
 *          for the mesh patterns (12, 309), (12, 345), (12, 465).
 * https://oeis.org/A289607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) - 2^(n-2) - (n-2)*(n-1)/2 (for n ≥ 2); a(0) =
 *            1, a(1) = 1
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
    if (n < 2)
        return catalan(n);            /* Catalan(0) = 1, Catalan(1) = 1 */

    seq_t cat = catalan(n);

    /* 2^(n-2) */
    seq_t p2 = pow2(n - 2);

    /* (n-2)*(n-1)/2 */
    idx_t m = n - 2;
    seq_t tri = (seq_t)((m * (m + 1)) / 2);   /* (n-2)*(n-1)/2 */

    seq_t diff = p2 + tri;

    return cat - diff;
}
