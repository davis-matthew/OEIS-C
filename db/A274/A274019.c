/*
 * A274019  Number of n-bead quaternary necklaces (no turning over allowed)
 *          that avoid the subsequence 110.
 * https://oeis.org/A274019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * Σ_{d|n} φ(n/d) * c(d), where c(1) = 4, c(2) = 16,
 *            c(3) = 61, and for n ≥ 4: c(n) = 4*c(n-1) – c(n-3). For n = 0,
 *            a(0) = 1. This implementation computes c(d) by a simple linear
 *            recurrence on-the-fly, avoiding any need for dynamic memory
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   calc_phi, compute_c
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/compute_c.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;                 /* paired divisor */
        seq_t c_d  = compute_c(d);
        seq_t c_e  = compute_c(e);
        sum += calc_phi((seq_t)e) * c_d; /* φ(e)·c(d)   */
        if (d != e) {
            sum += calc_phi((seq_t)d) * c_e; /* φ(d)·c(e)   */
        }
    }
    return sum / (seq_t)n;  /* division is exact */
}
