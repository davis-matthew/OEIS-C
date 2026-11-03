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
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t sn = n;                     /* cast index to seq_t */
    seq_t nm1 = sn - 1;               /* n-1 */
    seq_t term1 = nm1 * nm1;          /* (n-1)^2 */
    seq_t term2 = totient(n) * sigma_of(n);   /* phi(n) * sigma(n) */
    return term1 - term2;             /* difference */
}
