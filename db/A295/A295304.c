/*
 * A295304  a(n) = +1 if sigma(phi(n)) > n, -1 if sigma(phi(n)) < n, and 0 if
 *          sigma(phi(n)) = n, where phi = A000010 and sigma = A000203.
 * https://oeis.org/A295304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign(sigma(phi(n)) - n)
 * Range:     i8 n <= 60, i16 n <= 65536, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9719 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi = Euler's totient (phi) and sigma = sum‑of‑divisors (sigma_of). */

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

seq_t a(idx_t n)
{
    /* Compute φ(n).  phi returns a seq_t value. */
    seq_t phi_val   = totient((idx_t)n);
    /* Compute σ(φ(n)).  sigma_of expects an idx_t argument. */
    seq_t sigma_val = sigma_of((idx_t)phi_val);

    /* Return the sign of σ(φ(n)) – n. */
    if (sigma_val > (seq_t)n)
        return (seq_t)1;
    if (sigma_val < (seq_t)n)
        return (seq_t)-1;
    return (seq_t)0;
}
