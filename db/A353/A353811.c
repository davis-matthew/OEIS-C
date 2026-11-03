/*
 * A353811  a(n) = 1 if sigma(n) is odd, and has an odd number of prime
 *          factors (with multiplicity), otherwise 0. Here sigma is the sum of
 *          divisors function.
 * https://oeis.org/A353811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n) is odd and sigma(n) has an odd * number of
 *            prime factors (with multiplicity), otherwise 0 * (off‑by‑offset:
 *            a(1) corresponds to n = 1)
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 144 terms through n = 9648 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"

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
    if (n < 1u) {
        return 0;                /* undefined for n = 0 in OEIS */
    }

    /* Sigma of n */
    seq_t sig = sigma_of(n);

    /* Check that sigma(n) is odd */
    if ((sig % 2u) == 0u) {
        return 0;
    }

    /* Number of prime factors (with multiplicity) of sigma(n) */
    seq_t omega = bigomega_seq(sig);

    /* Return 1 iff this count is odd */
    return (omega % 2u) ? 1 : 0;
}
