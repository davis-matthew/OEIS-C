/*
 * A353680  a(n) = 1 if n is odd and phi(sigma(n)) is equal to phi(n),
 *          otherwise 0.
 * https://oeis.org/A353680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and phi(sigma(n)) == phi(n), otherwise 0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9383 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A353680 sequence term for a given n.
   Uses utility functions sigma_of and calc_phi provided in the build. */
#include <stdbool.h>
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
    /* Defensive: n should be >= 1, but we handle n == 0 gracefully. */
    if (n == 0)
        return 0;

    /* Return 0 immediately if n is even. */
    if ((n & 1) == 0)
        return 0;

    /* Compute sigma(n) and compare phi(sigma(n)) with phi(n). */
    seq_t sigma_n = sigma_of(n);        /* sigma(n) as seq_t */
    seq_t phi_sigma = totient_seq(sigma_n); /* phi(sigma(n)) */
    seq_t phi_n = totient_seq((seq_t)n);    /* phi(n) */

    return (phi_sigma == phi_n) ? 1 : 0;
}
