/*
 * A357622  Half-alternating sum of the reversed n-th composition in standard
 *          order.
 * https://oeis.org/A357622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = half-alternating sum of the reversed n-th composition in
 *            standard order
 * Range:     i8 n <= 81, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* positions of set bits in n (counted from 1, LSB). */
    idx_t pos[64];
    int pos_cnt = 0;
    idx_t bit_pos = 1;
    idx_t temp = n;
    while (temp) {
        if (temp & 1u) pos[pos_cnt++] = bit_pos;
        temp >>= 1;
        bit_pos++;
    }

    seq_t sum = 0;
    bool plus = true;          /* sign for the current pair (+ if true, - if false). */
    int pair_off = 0;          /* offset within the current pair (0 or 1). */
    idx_t prev = 0;

    for (int i = 0; i < pos_cnt; ++i) {
        idx_t curr = pos[i];
        idx_t diff = curr - prev;          /* first difference between consecutive positions. */

        if (plus) {
            sum += (seq_t)diff;
        } else {
            sum -= (seq_t)diff;
        }

        pair_off++;
        if (pair_off == 2) {
            pair_off = 0;
            plus = !plus;                 /* flip sign every two terms. */
        }

        prev = curr;
    }

    return sum;
}
