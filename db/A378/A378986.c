/*
 * A378986  a(n) = 2*phi(2*n) - 2*n, where phi is the Euler totient function.
 * https://oeis.org/A378986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*phi(2*n) - 2*n
 * Range:     i8 n <= 63, i16 n <= 28663, i32 n <= 32769, i64 n <= 32769,
 *            i128 n <= 32769, big any n
 * Validated: standard, pass, 90 terms through n = 9093 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function phi() from the utils library */
#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* Compute 2*n as seq_t to avoid overflow on very large values */
    seq_t twon = (seq_t)2 * (seq_t)n;

    /* phi expects an idx_t argument; cast is safe for all practical n */
    seq_t phi_val = totient((idx_t)twon);

    /* a(n) = 2*phi(2*n) - 2*n */
    return (seq_t)2 * phi_val - (seq_t)2 * (seq_t)n;
}
