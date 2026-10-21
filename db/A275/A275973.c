/*
 * A275973  A binary sequence due to Harold Jeffreys.
 * https://oeis.org/A275973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8960, u16 n <= 8960, u32 n <= 8960, u64 n <= 8960,
 *            u128 n <= 8960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275973: the binary sequence formed by alternating
 * blocks of 0's and 1's of lengths 1,1,2,4,8,16,….
 * a(n) = 1 for even‑indexed blocks, 0 for odd‑indexed blocks.
 * Implementation uses a simple loop that runs in O(log n) time. */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t t    = 0;   /* block index, starting at 0 */
    idx_t sum  = 0;   /* cumulative length of all previous blocks */
    idx_t len  = 1;   /* length of the current block */

    /* Skip blocks until n falls inside the current one. */
    while (n > sum + len) {
        sum += len;  /* finish current block */
        ++t;        /* move to next block */

        /* Lengths are 1, 1, 2, 4, 8, … */
        if (t == 1)          /* second block still length 1 */
            len = 1;
        else
            len <<= 1;       /* subsequent blocks double in size */
    }

    /* Even block indices emit 1, odd emit 0. */
    return ((t & 1) == 0) ? (seq_t)1 : (seq_t)0;
}
