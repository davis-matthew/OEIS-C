/*
 * A378987  Odd bisection of A083254, where A083254(n) = 2*phi(n)-n.
 * https://oeis.org/A378987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*phi(2*n-1) - (2*n-1)
 * Range:     i8 n <= 65, i16 n <= 16385, i32 n <= 32769, i64 n <= 32769,
 *            i128 n <= 32769, big any n
 * Validated: standard, pass, 100 terms through n = 9238 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* compute odd argument 2*n-1 */
    idx_t odd = 2 * n;
    odd--;                     /* odd = 2*n-1 */
    seq_t ph   = totient(odd);      /* phi is provided */
    return 2 * ph - (seq_t)odd;
}
