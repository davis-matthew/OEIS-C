/*
 * A297246  Down-variation of the base-16 digits of n; see Comments.
 * https://oeis.org/A297246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of all increases in the base‑16 digit chain of n
 *            (i.e., the down‑variation of the base‑16 digits)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return 0;   /* safe guard – not part of OEIS offset */

    /* least significant digit first */
    unsigned prev = n & 0xF;
    n >>= 4;
    for (; n != 0; n >>= 4) {
        unsigned curr = n & 0xF;
        if (curr > prev)
            sum += (seq_t)(curr - prev);
        prev = curr;
    }
    return sum;
}
