/*
 * A267844  a(n) = Catalan(n)^2*(4n + 3).
 * https://oeis.org/A267844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = Catalan(n)^2*(4n + 3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t c = catalan(n);                    /* Catalan number Cn */
    seq_t f = (seq_t)(4 * (seq_t)n + 3);     /* (4n + 3) as seq_t */
    seq_t res = c * c;                       /* Cn^2 */
    res *= f;                                /* * (4n + 3) */
    return res;
}
