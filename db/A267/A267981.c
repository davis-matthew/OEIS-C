/*
 * A267981  a(n) = Catalan(n)^2*(4n + 2).
 * https://oeis.org/A267981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = Catalan(n)^2*(4n + 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t c = catalan(n);                      /* Catalan(n) */
    seq_t result = c * c;                      /* Catalan(n)^2 */
    seq_t factor = ((seq_t)4 * (seq_t)n) + 2;   /* 4n + 2   */
    result *= factor;                          /* ×(4n+2)   */
    return result;
}
