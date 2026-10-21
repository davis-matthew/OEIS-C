/*
 * A267987  a(n) = Catalan(n)^2*(4n + 4).
 * https://oeis.org/A267987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = Catalan(n)^2*(4n + 4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     uses the provided catalan() function             */
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t cat   = catalan(n);          /* Catalan number C_n */
    seq_t result = cat * cat;          /* C_n^2                */
    seq_t mult   = (seq_t)n * 4 + 4;   /* 4n + 4 (promoted to seq_t)  */
    result *= mult;
    return result;
}
