/*
 * A259264  Cyclotomic polynomial value Phi(5,n!).
 * https://oeis.org/A259264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Φ₅(n!) = (n!)⁴ + (n!)³ + (n!)² + n! + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* for size_t */
#include "../utils/factorial.h"   /* provides factorial(idx_t) */

seq_t a(idx_t n)
{
    seq_t f  = factorial(n);    /* n!                     */
    seq_t f2 = f * f;           /* (n!)²                  */
    seq_t f3 = f2 * f;          /* (n!)³                  */
    seq_t f4 = f3 * f;          /* (n!)⁴                  */
    return f4 + f3 + f2 + f + 1; /* Φ₅(n!)                 */
}
