/*
 * A346692  a(n) = phi(n) - phi(n-phi(n)), a(1) = 1.
 * https://oeis.org/A346692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = phi(n) - phi(n - phi(n)), with a(1) = 1
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return 1;           /* a(1) = 1 */
    }
    /* compute phi(n) */
    seq_t phi_n = phi(n);
    /* compute n - phi(n) as an idx_t value (safe because phi(n) ≤ n) */
    idx_t m = n - (idx_t)phi_n;
    /* compute phi(n - phi(n)) */
    seq_t phi_m = phi(m);
    /* final result */
    return phi_n - phi_m;
}
