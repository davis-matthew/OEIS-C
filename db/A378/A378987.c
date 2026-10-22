/*
 * A378987  Odd bisection of A083254, where A083254(n) = 2*phi(n)-n.
 * https://oeis.org/A378987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*phi(2*n-1) - (2*n-1)
 * Range:     i8 n <= 64, i16 n <= 9238, i32 n <= 9238, i64 n <= 9238,
 *            i128 n <= 9238, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* compute odd argument 2*n-1 */
    idx_t odd = 2 * n;
    odd--;                     /* odd = 2*n-1 */
    seq_t ph   = phi(odd);      /* phi is provided */
    return 2 * ph - (seq_t)odd;
}
