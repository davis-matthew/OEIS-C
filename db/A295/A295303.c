/*
 * A295303  a(n) = +1 if n > phi(sigma(n)), -1 if n < phi(sigma(n)), and 0 if
 *          n = phi(sigma(n)), where phi = A000010 and sigma = A000203.
 * https://oeis.org/A295303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign(n - phi(sigma(n)))
 * Range:     i8 n <= 71, i16 n <= 65536, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9875 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient_seq.h"

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

seq_t a(idx_t n)
{
    /* n is >= 1 as per sequence definition */
    seq_t sigma = sigma_of(n);           /* sigma(n) */
    seq_t phi_sigma = totient_seq(sigma);   /* phi(sigma(n)) */
    seq_t N = (seq_t)n;                  /* cast index to seq_t for comparison */

    if (N > phi_sigma)
        return (seq_t)1;
    else if (N < phi_sigma)
        return (seq_t)-1;
    else
        return (seq_t)0;
}
