/*
 * A325887  Excess of sum of odd integers up to n and coprime to n over sum of
 *          even integers up to n and coprime to n.
 * https://oeis.org/A325887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n, gcd(k,n)=1} (-1)^(k+1)*k
 * Range:     i8 n <= 15, i16 n <= 375, i32 n <= 9546, i64 n <= 9546,
 *            i128 n <= 9546, big any n
 * Helpers:   mobius, phi, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Optimized implementation using known closed forms: */
/*   if n = 1: a(1) = 1 */
/*   if n even: a(n) = n*phi(n)/2 */
/*   if n odd (>1): a(n) = mu(rad(n))*phi(rad(n))/2 */

#include "../utils/phi.h"
#include "../utils/rad.h"
#include "../utils/mobius.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    /* Even n */
    if ((n & 1ULL) == 0) {
        seq_t phi_n = phi(n);
        seq_t prod = (seq_t)n * phi_n;
        return prod / 2;
    }

    /* Odd n > 1 */
    seq_t rad_n = rad(n);
    seq_t mu = mobius((idx_t)rad_n);
    seq_t phi_rad = phi((idx_t)rad_n);
    seq_t prod = mu * phi_rad;
    return prod / 2;
}
