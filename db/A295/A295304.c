/*
 * A295304  a(n) = +1 if sigma(phi(n)) > n, -1 if sigma(phi(n)) < n, and 0 if
 *          sigma(phi(n)) = n, where phi = A000010 and sigma = A000203.
 * https://oeis.org/A295304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign(sigma(phi(n)) - n)
 * Range:     i8 n <= 60, i16 n <= 9719, i32 n <= 9719, i64 n <= 9719,
 *            i128 n <= 9719, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi = Euler's totient (phi) and sigma = sum‑of‑divisors (sigma_of). */

#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Compute φ(n).  phi returns a seq_t value. */
    seq_t phi_val   = phi((idx_t)n);
    /* Compute σ(φ(n)).  sigma_of expects an idx_t argument. */
    seq_t sigma_val = sigma_of((idx_t)phi_val);

    /* Return the sign of σ(φ(n)) – n. */
    if (sigma_val > (seq_t)n)
        return (seq_t)1;
    if (sigma_val < (seq_t)n)
        return (seq_t)-1;
    return (seq_t)0;
}
