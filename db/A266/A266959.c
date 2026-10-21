/*
 * A266959  Smallest n-digit number ending in n.
 * https://oeis.org/A266959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n + 10^(n-1) for n > 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"     /* provides seq_t pow10(idx_t) */

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;
    seq_t ten_pow = pow10(n - 1);  /* 10^(n-1) */
    ten_pow += (seq_t)n;           /* add n */
    return ten_pow;
}
