/*
 * A249353  a(n) = phi(n) + sigma(sigma(n)).
 * https://oeis.org/A249353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) + sigma(sigma(n))
 * Range:     u8 n <= 41, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* n is guaranteed to be >= 1 */
    seq_t phi_n         = totient(n);                     /* φ(n)  */
    seq_t sigma_n       = sigma_of(n);                /* σ(n)  */
    /* sigma_n fits in idx_t for all n in the tested range */
    idx_t sigma_n_idx   = (idx_t)sigma_n;             /* cast for second σ-lookup */
    seq_t sigma_sigma_n = sigma_of(sigma_n_idx);      /* σ(σ(n)) */
    return phi_n + sigma_sigma_n;
}
