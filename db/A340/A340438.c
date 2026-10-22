/*
 * A340438  Number of sets in the geometry determined by the Hausdorff metric
 *          at each location between two sets defined by a complete bipartite
 *          graph K(5,n) (with n at least 4) missing three edges, where
 *          exactly two of the removed edges are incident to the same vertex
 *          in the 5-point set but none of the remov...
 * https://oeis.org/A340438
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3375*31^(n-3) - 3339*15^(n-3) + 1054*7^(n-3) -
 *            118*3^(n-3) + 3 (offset 4)
 * Range:     u8 none, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* For n < 4 the OEIS says the value is irrelevant */
    if (n < 4)
        return 0;

    /* exponent used in all powers */
    idx_t e = n - 3;

    /* Compute the required powers progressively.
       This is safe for any seq_t type that supports
       multiplication by small integer constants. */
    seq_t p31 = 1, p15 = 1, p7 = 1, p3 = 1;
    for (idx_t i = 0; i < e; ++i) {
        p31 *= (seq_t)31;
        p15 *= (seq_t)15;
        p7  *= (seq_t)7;
        p3  *= (seq_t)3;
    }

    seq_t term1 = (seq_t)3375 * p31;
    seq_t term2 = (seq_t)3339 * p15;
    seq_t term3 = (seq_t)1054 * p7;
    seq_t term4 = (seq_t)118   * p3;

    return term1 - term2 + term3 - term4 + (seq_t)3;
}
