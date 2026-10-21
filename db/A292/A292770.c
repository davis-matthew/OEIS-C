/*
 * A292770  If sigma(n)+phi(n) is even then a(n) = (sigma(n)+phi(n))/2
 *          otherwise -1.
 * https://oeis.org/A292770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (sigma(n)+phi(n))/2 if sigma(n)+phi(n) is even, otherwise
 *            -1 (offset 1)
 * Range:     i8 n <= 47, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"   /* declares seq_t sigma_of(idx_t)     */
#include "../utils/phi.h"        /* declares seq_t phi(idx_t)          */

seq_t a(idx_t n)
{
    /* values below the offset are undefined, return -1 to be safe */
    if (n < (idx_t)1)
        return (seq_t)-1;

    /* compute sigma(n) and phi(n) using the utility functions */
    seq_t sigma = sigma_of(n);
    seq_t phi_val = phi(n);

    /* sum and check parity */
    seq_t sum = sigma + phi_val;
    if ((sum % (seq_t)2) == (seq_t)0)
        return sum / (seq_t)2;
    else
        return (seq_t)-1;
}
