/*
 * A329687  Number of excursions of length n with Motzkin-steps avoiding the
 *          consecutive steps UU, UD, HH and DH.
 * https://oeis.org/A329687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = (n mod 3 == 2 ? 0 : catalan(n/3))
 * Range:     u8 n <= 17, u16 n <= 29, u32 n <= 53, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n) {
    if ((n % 3) == 2u) return 0;
    return catalan(n / 3);
}
