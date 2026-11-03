/*
 * A340461  a(n) = 2*sigma(phi(n)) - n.
 * https://oeis.org/A340461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * sigma(phi(n)) - n
 * Range:     u8 n <= 60, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0)
        return 0;                /* sequence is defined for n >= 1 */

    seq_t phi_n = totient_seq((seq_t)n);           /* φ(n) */
    seq_t sigma_phi = sigma_of((idx_t)phi_n);   /* σ(φ(n)) */
    return 2 * sigma_phi - (seq_t)n;            /* a(n) */
}
