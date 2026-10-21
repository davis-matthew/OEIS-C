/*
 * A280577  a(n) = eulerphi(n) + floor(n/2).
 * https://oeis.org/A280577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) + floor(n/2)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    /* compute the Euler totient φ(n) and add ⌊n/2⌋ */
    return phi(n) + (seq_t)(n / 2);
}
