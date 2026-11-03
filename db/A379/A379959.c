/*
 * A379959  a(n) = 1 if sigma(phi(n)) < n, otherwise 0.
 * https://oeis.org/A379959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(phi(n)) < n, otherwise 0
 * Range:     u8 n <= 108, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 142 terms through n = 9723 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0) /* outside the OEIS offset; return 0 */
        return 0;

    seq_t phi_val = totient_seq(n);          /* φ(n) */
    idx_t phi_idx = (idx_t)phi_val;       /* φ(n) ≤ n, fits in idx_t */
    seq_t sigma_val = sigma_of(phi_idx);  /* σ(φ(n)) */

    return (sigma_val < (seq_t)n) ? 1 : 0;
}
