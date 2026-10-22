/*
 * A379578  In the base-4 expansion of n map 0->1, 1->3, 2->0, 3->2.
 * https://oeis.org/A379578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) is obtained by mapping each base‑4 digit of n (or,
 *            equivalently, each pair of binary bits) via 0→1, 1→3, 2→0, 3→2.
 *            The result is read as a base‑4 number (i.e. the two binary bits
 *            of each mapped digit form the output)
 * Range:     u8 n <= 255, u16 n <= 9247, u32 n <= 9247, u64 n <= 9247,
 *            u128 n <= 9247, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned map4[4] = { 1, 3, 0, 2 };

seq_t a(idx_t n)
{
    /* Handle the special case n==0 separately because
       the number of bits in 0 is not defined by clz. */
    if (n == 0) {
        return (seq_t)1;   /* 0 → "00" → 01 (decimal 1) */
    }

    /* Determine number of bits in n.  clz returns
       leading zeros, so 64 - clz gives the bit length. */
    unsigned bits = 64 - __builtin_clzll(n);

    /* Ensure an even number of bits by prefixing a 0 if necessary. */
    if (bits & 1) {
        bits++;
    }

    unsigned pairs = bits >> 1;   /* bits / 2 */

    seq_t res = 0;

    /* Process each 2‑bit pair from the most significant end. */
    for (unsigned i = 0; i < pairs; ++i) {
        unsigned shift = 2 * (pairs - i - 1);
        unsigned pair  = (unsigned)((n >> shift) & 3u);
        unsigned mapped = map4[pair];
        res = (res << 2) | (seq_t)mapped;
    }

    return res;
}
