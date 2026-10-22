/*
 * A353810  a(n) = 1 if sigma(n) has an odd number of prime factors (with
 *          multiplicity), and 0 otherwise. Here sigma is the sum of divisors
 *          function.
 * https://oeis.org/A353810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n) has an odd number of prime factors (with
 *            multiplicity), else 0
 * Range:     u8 n <= 291, u16 n <= 6141, u32 n <= 6141, u64 n <= 6141,
 *            u128 n <= 6141, big any n
 * Helpers:   bigomega, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    idx_t sigma_idx = (idx_t)sigma;            /* sigma fits in 64‑bit for all intended n */
    seq_t omega   = bigomega(sigma_idx);
    return omega & 1;                          /* parity of the number of prime factors */
}
