/*
 * A291330  The arithmetic function v_4(n,1).
 * https://oeis.org/A291330
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - gcd(n,4)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Compute the value using the provided gcd routine. */
    seq_t sn = (seq_t)n;
    seq_t g  = gcd(sn, (seq_t)4);
    return sn - g;
}
