/*
 * A343228  A binary encoding of the digits "+1" in balanced ternary
 *          representation of n.
 * https://oeis.org/A343228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = binary pattern of the +1 digits in the balanced‐ternary
 *            representation of n
 * Range:     u8 n <= 3280, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m   = n;
    seq_t res = 0;
    idx_t bit = 0;
    while (m > 0) {
        uint64_t r = m % 3;
        m /= 3;
        if (r == 2) {          /* 2 ≡ –1 (mod 3) → carry 1 */
            m++;               /* propagate the carry to the next ternary digit */
        }
        if (r == 1) {          /* +1 digit → set the corresponding binary bit */
            res |= ((seq_t)1 << bit);
        }
        ++bit;
    }
    return res;
}
