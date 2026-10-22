/*
 * A302254  Exponent of the group of the Gaussian integers in a reduced system
 *          modulo (1+i)^n.
 * https://oeis.org/A302254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(floor(n/2)-1) for n > 5; a(0) = 1, a(1) = 1, a(2) = 2,
 *            a(3) = 4, a(4) = 4, a(5) = 4 (values for n <= 5 are hard‑coded).
 *            This matches the OEIS definition given in the entry. The
 *            calculation uses the power function pow2 from the utils library
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 257, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly. */
    if (n <= 5) {
        static const seq_t table[6] = {1, 1, 2, 4, 4, 4};
        return table[n];
    }
    /* For n > 5 apply the formula 2^(floor(n/2)-1). */
    idx_t exp = (n >> 1) - 1;   /* floor(n/2) - 1 */
    return pow2(exp);
}
