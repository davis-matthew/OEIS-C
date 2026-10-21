/*
 * A257589  a(n) = (2n+1)^2*Catalan(n).
 * https://oeis.org/A257589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n+1)^2 * Catalan(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* compute (2n+1) */
    seq_t two_n_plus_one = ((seq_t)n << 1) + 1;      /* (2n+1) */

    /* square it */
    seq_t square = two_n_plus_one * two_n_plus_one; /* (2n+1)^2 */

    /* Catalan number */
    seq_t cat = catalan(n);

    /* product */
    return square * cat;
}
