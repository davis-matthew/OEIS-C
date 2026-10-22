/*
 * A337855  Number of n-digit positive integers that are the product of two
 *          integers ending with 5.
 * https://oeis.org/A337855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 18 * 10^(n-3) for n > 2; a(1) = 0, a(2) = 2, a(3) = 18
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1) */
    if (n == 2) return 2;          /* a(2) */
    if (n == 3) return 18;         /* a(3) */
    /* n > 3 */
    return (seq_t)18 * pow10(n - 3);
}
