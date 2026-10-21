/*
 * A246960  Directions of the lines in the (Heighway) Dragon Curve.
 * https://oeis.org/A246960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th symbol in the fixed point of the morphism 0 →
 *            01, 1 → 21, 2 → 23, 3 → 03. Starting from symbol 0, for each
 *            binary bit of n (most significant first) take the left child if
 *            the bit is 0, or the right child if the bit is 1. The resulting
 *            symbol is a(n). This algorithm runs in O(log...
 * Range:     u8 n <= 9459, u16 n <= 9459, u32 n <= 9459, u64 n <= 9459,
 *            u128 n <= 9459, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t lmap[4] = {0, 2, 2, 0};
static const seq_t rmap[4] = {1, 1, 3, 3};

seq_t a(idx_t n)
{
    seq_t val = 0;          /* start from symbol 0 */
    if (n == 0)
        return val;

    /* count the number of bits needed to represent n */
    idx_t tmp = n;
    int bits = 0;
    while (tmp) {
        tmp >>= 1;
        bits++;
    }

    /* walk the bits from most significant to least significant */
    for (int i = bits - 1; i >= 0; --i) {
        if ((n >> i) & 1) {           /* bit == 1 → right child */
            val = rmap[(int)val];
        } else {                      /* bit == 0 → left child  */
            val = lmap[(int)val];
        }
    }
    return val;
}
