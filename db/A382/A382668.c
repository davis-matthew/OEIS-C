/*
 * A382668  a(n) = C(n+1) - C(n-1) - 2*C(n-2) where C(n) = A000108(n) are the
 *          Catalan numbers.
 * https://oeis.org/A382668
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+1) - C(n-1) - 2*C(n-2) where C(k) = A000108(k)
 *            (Catalan)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;       /* values below the offset are irrelevant */
    seq_t c_n_plus_1 = catalan(n + 1);
    seq_t c_n_minus_1 = catalan(n - 1);
    seq_t c_n_minus_2 = catalan(n - 2);
    seq_t two_c_n_minus_2 = c_n_minus_2 + c_n_minus_2;  /* 2*C(n-2) */
    seq_t sum = c_n_minus_1 + two_c_n_minus_2;
    return c_n_plus_1 - sum;
}
