/*
 * A342241  a(n) is the least k > 0 such that the first k bits and the last k
 *          bits in the binary expansion of n are the same.
 * https://oeis.org/A342241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the smallest k > 0 such that the first k bits of the *
 *            binary expansion of n equal the last k bits. For n=0 the answer
 *            is 1. * (If n is odd, a(n)=1; otherwise a(n) ≤ the length of the
 *            binary * representation of n.) The implementation runs in O(log
 *            n) time and * uses only integer arithmetic
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the number of bits required to represent n.
     * For n==0 the binary representation is "0" and we use length 1. */
    unsigned len = 0;
    idx_t t = n;
    while (t) {
        len++;
        t >>= 1;
    }
    if (len == 0) len = 1;  /* n == 0 */

    for (unsigned k = 1; k <= len; ++k) {
        /* Prefix: the upper k bits of n. */
        idx_t prefix = n >> (len - k);

        /* Mask for extracting the lower k bits.  Avoid shifting
         * by 64 or more which is undefined.  For k == 64 we use
         * a mask of all ones. */
        idx_t mask;
        if (k == 64) {
            mask = (idx_t)-1;          /* all 64 bits set */
        } else {
            mask = ((idx_t)1 << k) - 1;
        }
        idx_t suffix = n & mask;

        if (prefix == suffix) {
            return (seq_t)k;
        }
    }

    /* The loop always returns for k == len, but the compiler
     * needs a return here.  This line is unreachable. */
    return (seq_t)len;
}
