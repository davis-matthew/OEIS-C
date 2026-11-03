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
 * Range:     u8 n <= 291, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 6141 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

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
    seq_t sigma = sigma_of(n);
    idx_t sigma_idx = (idx_t)sigma;            /* sigma fits in 64‑bit for all intended n */
    seq_t omega   = bigomega(sigma_idx);
    return omega & 1;                          /* parity of the number of prime factors */
}
