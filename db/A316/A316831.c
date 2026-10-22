/*
 * A316831  Trajectory of 0 under repeated application of the morphism 0 ->
 *          01, 1 -> 21, 2 -> 13, 3 -> 33.
 * https://oeis.org/A316831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth symbol of the infinite word obtained by
 *            iterating the morphism 0→01, 1→21, 2→13, 3→33. The
 *            implementation follows the binary expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* child[s][b] gives the symbol obtained from symbol s
       when the next morphism step is taken and b indicates
       whether we pick the first (b=0) or second (b=1) character. */
    static const unsigned char child[4][2] = {
        {0, 1},   /* 0 → 01 */
        {2, 1},   /* 1 → 21 */
        {1, 3},   /* 2 → 13 */
        {3, 3}    /* 3 → 33 */
    };

    if (n == 0)
        return (seq_t)0;          /* a(0) = 0 */

    unsigned char s = 0;          /* start from the root symbol 0 */

    /* Find the highest set bit (most significant bit) of n. */
    idx_t msb = 0;
    for (idx_t i = 63; i > 0; --i) {
        if ((n >> i) & 1ULL) {
            msb = i;
            break;
        }
    }

    /* Walk from the most significant bit down to the least,
       applying the morphism at each step. */
    for (idx_t i = msb;; --i) {
        unsigned char bit = (unsigned char)((n >> i) & 1ULL);
        s = child[s][bit];
        if (i == 0)
            break;
    }

    return (seq_t)s;
}
