/*
 * A367833  a(n) = Product_{k=0..n} (n^8 + k^8).
 * https://oeis.org/A367833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (n⁸ + k⁸)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 2, a(2) = 33685504, ... */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special first term directly */
    if (n == 0)
        return (seq_t)0;

    /* Compute n⁸ once (must use seq_t arithmetic) */
    seq_t n_seq = (seq_t)n;
    seq_t n2   = n_seq * n_seq;   /* n² */
    seq_t n4   = n2   * n2;       /* n⁴ */
    seq_t n8   = n4   * n4;       /* n⁸ */

    seq_t prod = (seq_t)1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t k2   = k_seq * k_seq;   /* k² */
        seq_t k4   = k2   * k2;       /* k⁴ */
        seq_t k8   = k4   * k4;       /* k⁸ */

        seq_t term = n8 + k8;        /* n⁸ + k⁸ */
        prod *= term;                /* cumulative product */
    }

    return prod;
}
