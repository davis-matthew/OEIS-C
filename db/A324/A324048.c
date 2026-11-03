/*
 * A324048  a(n) = A000203(n) - A083254(n) = n + sigma(n) - 2*phi(n).
 * https://oeis.org/A324048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + sigma(n) - 2*phi(n)
 * Range:     u8 n <= 83, u16 n <= 15119, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 8609 (2026-11-01)
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
    seq_t n_val = (seq_t)n;
    seq_t sigma = sigma_of(n);    /* σ(n) */
    seq_t phi_val = totient(n);       /* φ(n) */

    return n_val + sigma - (phi_val << 1);
}
