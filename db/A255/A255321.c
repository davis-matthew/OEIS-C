/*
 * A255321  Product_{k=1..n} k^(k^3).
 * https://oeis.org/A255321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} k^(k^3) (OEIS A255321)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_large.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t exp = k * k * k;               /* k^3 fits in 64‑bit for all practical n */
        seq_t term = pow_seq_large((seq_t)k, (seq_t)exp);
        result *= term;                      /* multiplication is supported for seq_t */
    }
    return result;
}
