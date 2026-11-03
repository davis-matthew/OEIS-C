/*
 * A353637  a(n) = 1 if phi(sigma(n)) is equal to phi(n), otherwise 0.
 * https://oeis.org/A353637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff φ(σ(n)) == φ(n), otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 9873 (2026-11-01)
 * Helpers:   totient, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the required utility functions */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient.h"
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
    /* Compute σ(n) */
    seq_t sigma = sigma_of(n);

    /* Compute φ(σ(n)) and φ(n) */
    seq_t phi_sigma = totient_seq(sigma);
    seq_t phi_n = totient(n);

    /* Return 1 if the two values are equal, 0 otherwise */
    return (phi_sigma == phi_n) ? (seq_t)1 : (seq_t)0;
}
