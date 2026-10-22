/*
 * A330081  If the binary expansion of n is (b(1), ..., b(w)), then the binary
 *          expansion of a(n) is (b(1), b(3), b(5), ..., b(6), b(4), b(2)).
 * https://oeis.org/A330081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is formed from the binary representation of n by taking
 *            bits at odd positions (1,3,5,…) in order and then appending the
 *            bits at even positions (2,4,6,…) in reverse order. This
 *            implements OEIS sequence A330081
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* determine the number of bits of n (w) */
    idx_t w = 0;
    idx_t tmp = n;
    while (tmp) {
        w++;
        tmp >>= 1;
    }

    /* even_len = number of even positions, odd_len omitted */
    idx_t even_len = w / 2;

    seq_t odd_res = 0;        /* bits from odd positions, in order   */
    seq_t even_in = 0;        /* bits from even positions, as encountered */
    idx_t pos = 1;            /* position counting from the most
                                significant bit (1-based)                */

    while (pos <= w) {
        /* extract bit at position (w - pos) from the right  */
        idx_t shift = w - pos;
        seq_t bit = (seq_t)((n >> shift) & 1ULL);

        if ((pos & 1) == 1) {            /* odd position */
            odd_res = (odd_res << 1) | bit;
        } else {                         /* even position */
            even_in = (even_in << 1) | bit;
        }
        pos++;
    }

    /* reverse the order of the even bits */
    seq_t even_rev = 0;
    for (idx_t j = 0; j < even_len; j++) {
        seq_t bit = (even_in >> j) & 1ULL;
        even_rev = (even_rev << 1) | bit;
    }

    /* concatenate: odd bits followed by reversed even bits */
    return (odd_res << even_len) | even_rev;
}
