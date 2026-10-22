/*
 * A345892  a(n) = n + (n - 1) * (n - phi(n)).
 * https://oeis.org/A345892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - (n-1)*phi(n) (OEIS A345892)
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    /* phi(n) is computed by the provided helper */
    seq_t phi_val = phi(n);
    seq_t n_seq   = (seq_t)n;
    seq_t n_minus1 = (seq_t)(n - 1);
    return n_seq * n_seq - n_minus1 * phi_val;
}
