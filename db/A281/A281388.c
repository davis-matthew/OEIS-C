/*
 * A281388  Write n in binary reflected Gray code and sum the positions where
 *          there is a '1' followed immediately to the right by a '0',
 *          counting the leftmost digit as position 1.
 * https://oeis.org/A281388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of positions of '1' followed immediately by '0' in
 *            the binary reflected Gray code of n (positions counted from
 *            leftmost = 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Binary reflected Gray code of n */
    idx_t gray = n ^ (n >> 1);

    if (gray == 0)
        return (seq_t)0;

    /* Length of Gray code in bits */
    size_t len = 0;
    idx_t t = gray;
    while (t) {
        ++len;
        t >>= 1;
    }

    seq_t sum = 0;

    /* Most significant bit */
    bool prev = ((gray >> (len - 1)) & 1ULL) != 0;

    for (size_t pos = 1; pos < len; ++pos) {
        bool cur = ((gray >> (len - 1 - pos)) & 1ULL) != 0;
        if (prev && !cur)
            sum += (seq_t)pos;
        prev = cur;
    }

    return sum;
}
