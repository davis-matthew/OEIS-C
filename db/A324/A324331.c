/*
 * A324331  a(n) = (n-1)^2 - phi(n)*sigma(n), where phi is A000010 and sigma
 *          is A000203.
 * https://oeis.org/A324331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)^2 - phi(n)*sigma(n)
 * Range:     i8 n <= 29, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n) {
    seq_t sn = n;                     /* cast index to seq_t */
    seq_t nm1 = sn - 1;               /* n-1 */
    seq_t term1 = nm1 * nm1;          /* (n-1)^2 */
    seq_t term2 = phi(n) * sigma_of(n);   /* phi(n) * sigma(n) */
    return term1 - term2;             /* difference */
}
