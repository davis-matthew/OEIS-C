/*
 * A353636  Difference between phi(sigma(n)) and phi(n).
 * https://oeis.org/A353636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = phi(sigma(n)) - phi(n)
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   calc_phi, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);   /* sigma(n) */
    seq_t phis = calc_phi(s);/* phi(sigma(n)) */
    seq_t phin = phi(n);     /* phi(n) */
    return phis - phin;
}
