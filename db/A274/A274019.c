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
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_seq.h"

/* Compute the recurrence value c(d). */
static seq_t compute_c(idx_t d)
{
    if (d == 1) return (seq_t)4;
    if (d == 2) return (seq_t)16;
    if (d == 3) return (seq_t)61;

    seq_t c_nm3 = (seq_t)4;   /* c(1)   */
    seq_t c_nm2 = (seq_t)16;  /* c(2)   */
    seq_t c_nm1 = (seq_t)61;  /* c(3)   */
    seq_t c_n   = 0;

    for (idx_t i = 4; i <= d; ++i) {
        c_n   = 4 * c_nm1 - c_nm3;
        c_nm3 = c_nm2;
        c_nm2 = c_nm1;
        c_nm1 = c_n;
    }
    return c_nm1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;                 /* paired divisor */
        seq_t c_d  = compute_c(d);
        seq_t c_e  = compute_c(e);
        sum += totient_seq((seq_t)e) * c_d; /* φ(e)·c(d)   */
        if (d != e) {
            sum += totient_seq((seq_t)d) * c_e; /* φ(d)·c(e)   */
        }
    }
    return sum / (seq_t)n;  /* division is exact */
}
