/*
 * A323659  a(n) = Product_{k=0..n} (k^10 + (n-k)^10).
 * https://oeis.org/A323659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (k^10 + (n-k)^10)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t mid = n / 2;          /* floor division, handles n even/odd */
    for (idx_t k = 0; k <= mid; ++k) {
        seq_t k_s = (seq_t)k;
        seq_t nk_s = (seq_t)(n - k);

        /* Compute k^10 */
        seq_t k10 = 1;
        for (idx_t i = 0; i < 10; ++i) {
            k10 *= k_s;
        }

        /* Compute (n-k)^10 */
        seq_t nk10 = 1;
        for (idx_t i = 0; i < 10; ++i) {
            nk10 *= nk_s;
        }

        /* (k^10 + (n-k)^10) */
        seq_t term = k10 + nk10;

        if (k == n - k) {
            /* middle term: multiply once */
            result *= term;
        } else {
            /* pair of equal terms: multiply twice */
            result *= term;
            result *= term;
        }
    }
    return result;
}
